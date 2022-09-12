/**
 *    GenshiCraft. Play Genshin Impact in Minecraft!
 *    Copyright (C) 2022  Futrime <futrime@outlook.com>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Affero General Public License as published
 *    by the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * @file actorex.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the ActorEx class
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "actorex.h"

#include <MC/Actor.hpp>
#include <MC/ActorUniqueID.hpp>
#include <MC/Level.hpp>
#include <algorithm>
#include <memory>
#include <random>
#include <third-party/Base64/Base64.hpp>
#include <third-party/Nlohmann/json.hpp>
#include <vector>

#include "damage.h"
#include "exceptions.h"
#include "mobex.h"
#include "plugin.h"
#include "stats.h"
#include "world.h"

namespace genshicraft {

ActorEx::ActorEx(Actor* actor)
    : attached_element_list_(),
      level_(0),
      stats_(Stats()),
      unique_id_(actor->getUniqueID().get()) {
  // Empty
}

ActorEx::~ActorEx() { this->SaveData(); }

void ActorEx::AddAttachedElement(struct world::ElementGaugeUnit gauge) {
  for (auto& attached_gauge : this->attached_element_list_) {
    if (attached_gauge.element != gauge.element) {
      attached_gauge.expiration = GetNowClock() - 1.;  // remove other gauges
    }
  }
  this->attached_element_list_.push_back(gauge);
}

Actor* ActorEx::GetActor() const {
  return Level::getEntity(ActorUniqueID(this->GetUniqueID()));
}

Damage ActorEx::GetAttackDamage() const {
  Damage damage;
  damage.SetAttackElementType(world::ElementType::kPhysical);
  damage.SetAttackerAmplifier(1.);
  damage.SetAttackerLevel(this->GetLevel());
  damage.SetAttackerStats(this->GetStats());
  damage.SetSourceType(Damage::SourceType::kMob);
  return damage;
}

std::vector<struct world::ElementGaugeUnit> ActorEx::GetAllAttachedElements()
    const {
  // Get the attached element list
  std::vector<struct world::ElementGaugeUnit> gauge_list;
  for (const auto& attached_gauge : this->attached_element_list_) {
    for (auto& gauge : gauge_list) {
      if (attached_gauge.element == gauge.element) {
        gauge.gauge += attached_gauge.gauge;
        continue;
      }
    }
    gauge_list.push_back(attached_gauge);
  }

  // Sort the attached element list
  auto cmp = [](world::ElementGaugeUnit a, world::ElementGaugeUnit b) {
    return a.gauge >= b.gauge;  // descending
  };
  std::sort(gauge_list.begin(), gauge_list.end(), cmp);

  return gauge_list;
}

int ActorEx::GetLevel() const { return this->level_; }

Stats ActorEx::GetStats() const { return this->stats_; }

long long ActorEx::GetUniqueID() const { return this->unique_id_; }

bool ActorEx::IsMob() const { return false; }

bool ActorEx::IsPlayer() const { return false; }

void ActorEx::SetATKByNativeDamage(double native_damage) {
  this->stats_.ATK_base = static_cast<int>(
      native_damage * world::GetEnemyATKMultiplier(this->GetLevel()) * 18.);
}

std::shared_ptr<ActorEx> ActorEx::Get(long long unique_id) {
  auto actor = Level::getEntity(ActorUniqueID(unique_id));

  return ActorEx::Make(actor);
}

std::shared_ptr<ActorEx> ActorEx::Make(Actor* actor) {
  if (actor == nullptr) {
    return std::shared_ptr<ActorEx>();
  }

  auto mobex = MobEx::Get(actor->getUniqueID().get());
  if (mobex) {
    return mobex;
  }

  auto actorex = std::make_shared<ActorEx>(actor);

  actorex->LoadData();

  return actorex;
}

void ActorEx::OnTick() {
  auto now_clock = GetNowClock();

  for (auto&& actor : Level::getAllEntities()) {
    // Do not process players
    if (actor->getTypeName() == "minecraft:player") {
      continue;
    }

    auto actorex = ActorEx::Make(actor);

    // Maintain the attached elements
    // Add new gauges (the priority matters)
    if (actor->isOnFire()) {  // 1#
      actorex->AddAttachedElement(
          {world::ElementType::kPyro, now_clock + 30., 1.});
    }
    if (actor->isInWaterOrRain()) {  // 2#
      actorex->AddAttachedElement(
          {world::ElementType::kHydro, now_clock + 30., 1.});
    }
    // Remove expired gauges
    for (auto it = actorex->attached_element_list_.begin();
         it != actorex->attached_element_list_.end();) {
      if (it->expiration < now_clock) {
        actorex->attached_element_list_.erase(it);
      } else {
        ++it;
      }
    }

    // Mob dedicated processes
    // Maintain the native healing
    if (actorex->IsMob()) {
      auto mobex = std::dynamic_pointer_cast<MobEx>(actorex);
      if (mobex->GetLastNativeHealth() < actor->getHealth()) {
        mobex->IncreaseHP(static_cast<int>(
            (actor->getHealth() - mobex->GetLastNativeHealth()) *
            (1. * mobex->GetStats().GetMaxHP() / actor->getMaxHealth())));
      }
      mobex->SetLastNativeHealth(actor->getHealth());
    }
  }
}

void ActorEx::LoadData() {
  static std::default_random_engine random_engine;

  auto actor = this->GetActor();

  if (actor == nullptr) {
    throw ExceptionActorNotFound();
  }

  nlohmann::json data;  // the actor data

  // Attempt to get the data from tags of the Actor object
  for (const auto& tag : actor->getAllTags()) {
    if (tag.substr(0, 12) == "genshicraft_") {
      try {
        data = nlohmann::json::parse(Base64::Decode(tag.substr(12)));

      } catch (const nlohmann::json::parse_error&) {
        actor->removeTag(tag);  // remove the malformed tag
        continue;
      }

      actor->removeTag(
          tag);  // the tag will be written back at the end of this method
      break;
    }
  }

  // Remove all GenshiCraft data tags
  for (const auto& tag : actor->getAllTags()) {
    if (tag.substr(0, 12) == "genshicraft_") {
      actor->removeTag(tag);
    }
  }

  // Migrate the data and check its validity
  data = ActorEx::MigrateData(data);

  // Initialize the actor with template data if the actor is not initialized or
  // the data is invalid
  if (data.empty()) {
    std::uniform_int_distribution dist(-10, 1);

    data["version"] = ActorEx::kActorExDataFormatVersion;

    data["level"] =
        world::GetWorldLevel(actor->getPosition(), actor->getDimension()) * 11 +
        dist(random_engine);

    data["attached_element_list"] = nlohmann::json::array();

    // Write the data to a tag of the Actor object
    auto tag = Base64::Encode(nlohmann::to_string(data));
    tag = tag.substr(
        0, tag.find(
               '='));  // remove the "=" characters which are forbidden in tags
    actor->addTag("genshicraft_" + tag);
  }

  // Load the data
  this->level_ = data["level"];

  for (const auto& gauge_json : data["attached_element_list"]) {
    struct world::ElementGaugeUnit gauge = {
        static_cast<world::ElementType>(gauge_json["element"].get<int>()),
        gauge_json["expiration"].get<double>(),
        gauge_json["gauge"].get<double>()};
    this->attached_element_list_.push_back(gauge);
  }
}

void ActorEx::SaveData() {
  if (this->is_data_saved_) {
    return;
  }

  this->is_data_saved_ = true;

  auto actor = this->GetActor();

  // Nothing to do if the mob no longer exists
  if (actor == nullptr) {
    return;
  }

  // Remove all GenshiCraft data tags
  for (const auto& tag : actor->getAllTags()) {
    if (tag.substr(0, 12) == "genshicraft_") {
      actor->removeTag(tag);
    }
  }

  nlohmann::json data;
  data["version"] = MobEx::kActorExDataFormatVersion;
  data["level"] = this->level_;  // the data value should be the most original
                                 // value so do not use this->GetLevel()
  data["attached_element_list"] = nlohmann::json::array();
  for (const auto& gauge : this->attached_element_list_) {
    nlohmann::json gauge_json;
    gauge_json["element"] = static_cast<int>(gauge.element);
    gauge_json["expiration"] = gauge.expiration;
    gauge_json["gauge"] = gauge.gauge;

    data["attached_element_list"].push_back(gauge_json);
  }

  // Write the data to a tag of the Mob object
  auto tag = Base64::Encode(nlohmann::to_string(data));
  tag = tag.substr(
      0,
      tag.find('='));  // remove the "=" characters which are forbidden in tags
  actor->addTag("genshicraft_" + tag);
}

nlohmann::json ActorEx::MigrateData(const nlohmann::json& old_data) {
  nlohmann::json data = old_data;

  try {
    // Migrate to version 1
    if (!old_data.contains("version")) {
      data["version"] = 1;
    }

    // Check the accessibility
    data["version"].get<int>();
    data["level"].get<int>();

    for (auto& gauge_json : data["attached_element_list"]) {
      gauge_json["element"].get<int>();
      gauge_json["expiration"].get<double>();
      gauge_json["gauge"].get<double>();
    }

  } catch (const nlohmann::json::exception&) {
    // Return an empty JSON object if the data is invalid
    return nlohmann::json();
  }

  return data;
}

}  // namespace genshicraft