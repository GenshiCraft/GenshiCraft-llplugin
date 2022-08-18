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
 * @file plugin.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of global interfaces and objects
 * @version 1.0.0
 * @date 2022-07-22
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "plugin.h"

#include <EventAPI.h>
#include <LoggerAPI.h>
#include <ScheduleAPI.h>
#include <ServerAPI.h>

#include <MC/AbstractArrow.hpp>
#include <MC/Actor.hpp>
#include <MC/ActorDamageSource.hpp>
#include <MC/Container.hpp>
#include <MC/ItemStack.hpp>
#include <MC/Player.hpp>
#include <MC/Types.hpp>
#include <chrono>
#include <cmath>
#include <map>
#include <random>
#include <third-party/Base64/Base64.hpp>
#include <third-party/Nlohmann/json.hpp>

#include "artifact.h"
#include "character.h"
#include "command.h"
#include "damage.h"
#include "exceptions.h"
#include "food.h"
#include "playerex.h"
#include "stats.h"
#include "version.h"
#include "weapon.h"
#include "world.h"

namespace genshicraft {

Logger logger(PLUGIN_NAME);

void CheckProtocolVersion() {
#ifdef TARGET_BDS_PROTOCOL_VERSION
  auto currentProtocol = LL::getServerProtocolVersion();
  if (TARGET_BDS_PROTOCOL_VERSION != currentProtocol) {
    logger.warn(
        "Protocol version not match, target version: {}, current version: {}.",
        TARGET_BDS_PROTOCOL_VERSION, currentProtocol);
    logger.warn(
        "This will most likely crash the server, please use the Plugin that "
        "matches the BDS version!");
  }
#endif  // TARGET_BDS_PROTOCOL_VERSION
}

double GetNowClock() {
  return static_cast<double>(
             std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::steady_clock::now().time_since_epoch())
                 .count()) /
         1000;
}

void Init() {
  CheckProtocolVersion();

  Command::Init();
  PlayerEx::Init();

  Event::MobHurtEvent::subscribe_ref(OnMobHurt);
  Event::PlayerDropItemEvent::subscribe_ref(OnPlayerDropItem);
  Event::PlayerExperienceAddEvent::subscribe_ref(OnPlayerExperienceAdd);
  Event::PlayerInventoryChangeEvent::subscribe_ref(OnPlayerInventoryChange);
  Event::PlayerJoinEvent::subscribe_ref(OnPlayerJoin);
  Event::PlayerLeftEvent::subscribe_ref(OnPlayerLeft);
  Event::PlayerOpenContainerEvent::subscribe_ref(OnPlayerOpenContainer);
  Event::PlayerOpenContainerScreenEvent::subscribe_ref(
      OnPlayerOpenContainerScreen);
  Event::PlayerRespawnEvent::subscribe_ref(OnPlayerRespawn);
  Event::PlayerUseItemEvent::subscribe_ref(OnPlayerUseItem);

  Schedule::repeat(OnTick, 1);
}

bool OnMobHurt(Event::MobHurtEvent& event) {
  static std::default_random_engine random_engine;
  static std::uniform_int_distribution dist(-10, 1);

  int world_level = world::GetWorldLevel(event.mMob->getPosition(),
                                         event.mMob->getDimension());

  Damage damage;

  // The damage source
  Player* player_attacker = nullptr;

  if ((event.mDamageSource->isEntitySource() &&
       event.mDamageSource->getEntity()->getTypeName() == "minecraft:player") ||
      (event.mDamageSource->isEntitySource() &&
       (event.mDamageSource->getEntity()->getTypeName() == "minecraft:arrow" ||
        event.mDamageSource->getEntity()->getTypeName() ==
            "minecraft:thrown_trident") &&
       static_cast<AbstractArrow*>(event.mDamageSource->getEntity())
           ->isPlayerOwned())) {  // if the damage is caused by a player

    Player* player;
    if (event.mDamageSource->getEntity()->getTypeName() ==
        "minecraft:player") {  // if the damage is from melee attack

      player = static_cast<Player*>(event.mDamageSource->getEntity());
      auto playerex = PlayerEx::Get(player->getXuid());

      if (player->getHandSlot()->getTypeName() == "minecraft:wooden_sword" ||
          player->getHandSlot()->getTypeName() == "minecraft:stone_sword" ||
          player->getHandSlot()->getTypeName() == "minecraft:golden_sword" ||
          player->getHandSlot()->getTypeName() == "minecraft:iron_sword" ||
          player->getHandSlot()->getTypeName() == "minecraft:diamond_sword" ||
          player->getHandSlot()->getTypeName() == "minecraft:netherite_sword" ||
          player->getHandSlot()->getTypeName() == "minecraft:wooden_axe" ||
          player->getHandSlot()->getTypeName() == "minecraft:stone_axe" ||
          player->getHandSlot()->getTypeName() == "minecraft:golden_axe" ||
          player->getHandSlot()->getTypeName() == "minecraft:iron_axe" ||
          player->getHandSlot()->getTypeName() == "minecraft:diamond_axe" ||
          player->getHandSlot()->getTypeName() == "minecraft:netherite_axe" ||
          player->getHandSlot()->getTypeName() == "minecraft:wooden_pickaxe" ||
          player->getHandSlot()->getTypeName() == "minecraft:stone_pickaxe" ||
          player->getHandSlot()->getTypeName() == "minecraft:golden_pickaxe" ||
          player->getHandSlot()->getTypeName() == "minecraft:iron_pickaxe" ||
          player->getHandSlot()->getTypeName() == "minecraft:diamond_pickaxe" ||
          player->getHandSlot()->getTypeName() ==
              "minecraft:netherite_pickaxe" ||
          player->getHandSlot()->getTypeName() == "minecraft:wooden_shovel" ||
          player->getHandSlot()->getTypeName() == "minecraft:stone_shovel" ||
          player->getHandSlot()->getTypeName() == "minecraft:golden_shovel" ||
          player->getHandSlot()->getTypeName() == "minecraft:iron_shovel" ||
          player->getHandSlot()->getTypeName() == "minecraft:diamond_shovel" ||
          player->getHandSlot()->getTypeName() ==
              "minecraft:netherite_shovel" ||
          player->getHandSlot()->getTypeName() == "minecraft:wooden_hoe" ||
          player->getHandSlot()->getTypeName() == "minecraft:stone_hoe" ||
          player->getHandSlot()->getTypeName() == "minecraft:golden_hoe" ||
          player->getHandSlot()->getTypeName() == "minecraft:iron_hoe" ||
          player->getHandSlot()->getTypeName() == "minecraft:diamond_hoe" ||
          player->getHandSlot()->getTypeName() == "minecraft:netherite_hoe" ||
          player->getHandSlot()->getTypeName() ==
              "minecraft:trident") {  // if the damage is from Minecraft weapons

        Stats stats;
        stats.ATK_base = static_cast<int>(
            event.mDamage * playerex->GetCharacter()->GetStats().GetATK() *
            0.1974);

        damage.SetStats(stats);
        damage.SetLevel(playerex->GetCharacter()->GetLevel());

      } else {  // if the damage is not from Minecraft weapons

        damage = playerex->GetCharacter()->GetDamageNormalAttack();
      }

    } else {  // if the damage is from thrown trident or arrow

      player = static_cast<AbstractArrow*>(event.mDamageSource->getEntity())
                   ->getPlayerOwner();
      auto playerex = PlayerEx::Get(player->getXuid());

      Stats stats;
      stats.ATK_base = static_cast<int>(
          event.mDamage * playerex->GetCharacter()->GetStats().GetATK() *
          0.1974);

      damage.SetStats(stats);
      damage.SetLevel(playerex->GetCharacter()->GetLevel());
    }

    player_attacker = player;  // for damage value displaying

  } else if (event.mDamageSource->isEntitySource() &&
             event.mDamageSource->getEntity()->getTypeName().substr(0, 10) ==
                 "minecraft:") {  // if the damage is caused by a Minecraft mob

    nlohmann::json data;  // the mob data

    // Get the data from the mob
    for (const auto& tag : event.mDamageSource->getEntity()->getAllTags()) {
      if (tag.substr(0, 12) ==
          "genshicraft_") {  // the tag with "genshicraft_" prefix is used for
                             // GenshiCraft entity data storage
        data = nlohmann::json::parse(Base64::Decode(tag.substr(12)));
        event.mDamageSource->getEntity()->removeTag(
            tag);  // remove the tag for further processing
        break;
      }
    }

    // Initialize the mob
    if (data.empty()) {
      data["level"] = world_level * 11 + dist(random_engine);  // the mob level

      data["ATK"] = 0;  // awaiting for further initialization
      data["max_HP"] = static_cast<int>(
          event.mDamageSource->getEntity()->getMaxHealth() *
          world::GetEnemyMaxHPMultiplier(data["level"].get<int>()) * 3.65);
      data["HP"] =
          static_cast<int>(data["max_HP"].get<double>() *
                           event.mDamageSource->getEntity()->getHealth() /
                           event.mDamageSource->getEntity()->getMaxHealth());
    }
    if (data["ATK"].get<int>() ==
        0) {  // if the ATK of the mob has not been initialized
      data["ATK"] = static_cast<int>(
          event.mDamage *
          world::GetEnemyATKMultiplier(data["level"].get<int>()) *
          18);  // the ATK of the zombie should be
                // the same as that of the hilichurl
    }

    Stats stats;
    stats.ATK_base = data["ATK"].get<int>();
    stats.DEF_base = data["level"].get<int>() * 5 + 500;
    stats.max_HP_base = data["max_HP"].get<int>();

    damage.SetStats(stats);
    // damage.SetElementType();
    damage.SetLevel(data["level"].get<int>());

    // Write the data to the mob
    auto tag = Base64::Encode(nlohmann::to_string(data));
    tag = tag.substr(0, tag.find('='));
    event.mDamageSource->getEntity()->addTag("genshicraft_" + tag);

  } else {  // if the damage is caused by the environment

    Stats stats;
    stats.ATK_base = static_cast<int>(event.mDamage);

    damage.SetStats(stats);
    // damage.SetElementType();
    damage.SetLevel(world_level * 11 - 10);
    damage.SetSourceType(Damage::SourceType::kEnvironment);
  }

  // The victim
  int victim_HP = 0;
  int victim_max_HP = 0;

  if (event.mMob->getTypeName() ==
      "minecraft:player") {  // if the victim is a player
    auto player = static_cast<Player*>(event.mMob);
    auto playerex = PlayerEx::Get(player->getXuid());

    damage.SetVictimStats(playerex->GetCharacter()->GetStats());
    // damage.SetVictimElementType();
    damage.SetVictimLevel(playerex->GetCharacter()->GetLevel());

    double damage_value = damage.Get();

    // Environment damage
    if (damage.GetSourceType() == Damage::SourceType::kEnvironment) {
      damage_value *=
          static_cast<double>(playerex->GetCharacter()->GetStats().GetMaxHP()) /
          player->getMaxHealth();

      // Prevent the too frequent damage
      if (event.mDamageSource->getCause() == ActorDamageCause::Contact ||
          event.mDamageSource->getCause() == ActorDamageCause::Fire ||
          event.mDamageSource->getCause() == ActorDamageCause::FireTick ||
          event.mDamageSource->getCause() == ActorDamageCause::Lava) {
        damage_value /= 20;
      }
    }

    playerex->GetCharacter()->IncreaseHP(
        static_cast<int>(-std::ceil(damage_value)));  // damage the character
    event.mDamage = 0;  // not to damage the Minecraft player

    victim_HP = playerex->GetCharacter()->GetHP();
    victim_max_HP = playerex->GetCharacter()->GetStats().GetMaxHP();

  } else if (event.mMob->getTypeName().substr(0, 10) ==
             "minecraft:") {  // if the victim is a Minecraft mob
    nlohmann::json data;

    // Get the data from the mob
    for (const auto& tag : event.mMob->getAllTags()) {
      if (tag.substr(0, 12) == "genshicraft_") {
        data = nlohmann::json::parse(Base64::Decode(tag.substr(12)));
        event.mMob->removeTag(tag);  // remove the tag for further processing
        break;
      }
    }

    // Use template data if the mob has not been initialized
    if (data.empty()) {
      data["level"] = world_level * 11 + dist(random_engine);  // the mob level

      data["ATK"] = 0;  // awaiting for further initialization
      data["max_HP"] = static_cast<int>(
          event.mMob->getMaxHealth() *
          world::GetEnemyMaxHPMultiplier(data["level"].get<int>()) * 3.65);
      data["HP"] = static_cast<int>(data["max_HP"].get<double>() *
                                    event.mMob->getHealth() /
                                    event.mMob->getMaxHealth());
    }

    Stats stats;
    stats.ATK_base = data["ATK"].get<int>();
    stats.DEF_base = data["level"].get<int>() * 5 + 500;
    stats.max_HP_base = data["max_HP"].get<int>();

    damage.SetVictimStats(stats);
    // damage.SetVictimElementType();
    damage.SetVictimLevel(data["level"].get<int>());

    // Ensure that the mob can be healed
    if (data["HP"].get<int>() * event.mMob->getMaxHealth() /
            data["max_HP"].get<int>() <
        event.mMob->getHealth()) {
      data["HP"] = static_cast<int>(
          data["HP"].get<int>() +
          (event.mMob->getHealth() - data["HP"].get<int>() *
                                         event.mMob->getMaxHealth() /
                                         data["max_HP"].get<int>()) *
              (data["max_HP"].get<double>() / event.mMob->getMaxHealth()));
    }

    double damage_value = damage.Get();

    // Environment damage
    if (damage.GetSourceType() == Damage::SourceType::kEnvironment) {
      damage_value *= data["max_HP"].get<double>() / event.mMob->getMaxHealth();

      // Prevent the too frequent damage
      if (event.mDamageSource->getCause() == ActorDamageCause::Contact ||
          event.mDamageSource->getCause() == ActorDamageCause::Fire ||
          event.mDamageSource->getCause() == ActorDamageCause::FireTick ||
          event.mDamageSource->getCause() == ActorDamageCause::Lava) {
        damage_value /= 20;
      }
    }

    // Damage the mob
    data["HP"] =
        static_cast<int>(data["HP"].get<int>() - std::ceil(damage_value));
    event.mDamage = static_cast<float>(
        event.mMob->getHealth() -
        (data["HP"].get<double>() / data["max_HP"].get<double>() *
         event.mMob->getMaxHealth()));
    if (data["HP"].get<int>() <= 0) {
      event.mDamage = 999999;  // kill the mob instantly
    }

    victim_HP = data["HP"].get<int>();
    victim_max_HP = data["max_HP"].get<int>();

    // Write the data to the mob
    auto data_str = nlohmann::to_string(data);
    auto tag = Base64::Encode(data_str);
    tag = tag.substr(0, tag.find('='));
    event.mMob->addTag("genshicraft_" + tag);
  }

  // Zero-damage hurt indicates failed hurt
  if (damage.Get() < 0.0001) {
    return false;
  }

  // Show the damage value at the action bar
  if (player_attacker != nullptr) {
    static const std::map<Damage::ElementType, std::string> kElementTypeColor =
        {
            {Damage::ElementType::kAnemo, "§3"},
            {Damage::ElementType::kCryo, "§b"},
            {Damage::ElementType::kDendro, "§a"},
            {Damage::ElementType::kElectro, "§d"},
            {Damage::ElementType::kGeo, "§g"},
            {Damage::ElementType::kHydro, "§9"},
            {Damage::ElementType::kPhysical, "§f"},
            {Damage::ElementType::kPyro, "§c"},
        };

    player_attacker->sendTitlePacket(
        kElementTypeColor.at(damage.GetElementType()) +
            std::to_string(static_cast<int>(damage.Get())) + " §f(" +
            std::to_string(std::max(victim_HP, 0)) + "§7/" +
            std::to_string(victim_max_HP) + "§f)",
        TitleType::SetActionBar, 0, 1, 0);
  }

  return true;
}

bool OnPlayerDropItem(Event::PlayerDropItemEvent& event) {
  // Prevent keeping inventory
  if (event.mPlayer->getHealth() == 0) {
    return true;
  }

  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  if (!playerex->GetPlayer()
           ->isSneaking()) {  // if the player is pressing Q without Shift
    if (playerex->GetCharacter()->HasWeapon()) {
      // Perform elemental burst

      return false;
    }
  } else {  // if the player is pressing Shift + Q
    playerex->GetMenu().OpenMain();

    return false;
  }

  return true;
}

bool OnPlayerExperienceAdd(Event::PlayerExperienceAddEvent& event) {
  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  playerex->GetCharacter()->IncreaseEnergy(event.mExp);

  return true;
}

bool OnPlayerRespawn(Event::PlayerRespawnEvent& event) {
  PlayerEx::OnPlayerRespawn(event.mPlayer);

  return true;
}

bool OnPlayerInventoryChange(Event::PlayerInventoryChangeEvent& event) {
  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  if (!playerex) {  // if the player is not loaded
    return true;
  }

  if (Weapon::CheckIsWeapon(event.mNewItemStack)) {
    Weapon::Make(event.mNewItemStack, playerex.get());
  }

  if (Artifact::CheckIsArtifact(event.mNewItemStack)) {
    Artifact::Make(event.mNewItemStack, playerex.get());
  }

  if (food::CheckIsFood(event.mNewItemStack)) {
    food::RegisterFood(playerex.get(), event.mNewItemStack);
  }

  return true;
}

bool OnPlayerJoin(Event::PlayerJoinEvent& event) {
  PlayerEx::LoadPlayer(event.mPlayer);

  return true;
}

bool OnPlayerLeft(Event::PlayerLeftEvent& event) {
  PlayerEx::UnloadPlayer(event.mPlayer);

  return true;
}

bool OnPlayerOpenContainer(Event::PlayerOpenContainerEvent& event) {
  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  playerex->SetIsOpeningContainer(true);

  return true;
}

bool OnPlayerOpenContainerScreen(Event::PlayerOpenContainerScreenEvent& event) {
  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  if (!playerex->IsOpeningContainer() &&
      !playerex->GetPlayer()->isSneaking()) {  // if the player is pressing E
                                               // without pressing Shift
    if (playerex->GetCharacter()->HasWeapon()) {
      // Perform elemental skill

      return false;
    }
  }

  playerex->SetIsOpeningContainer(false);
  return true;
}

bool OnPlayerUseItem(Event::PlayerUseItemEvent& event) {
  if (food::CheckIsFood(event.mItemStack)) {
    auto playerex = PlayerEx::Get(event.mPlayer->getXuid());

    return food::EatFood(playerex.get(), event.mItemStack);
  }

  return true;
}

void OnTick() { PlayerEx::OnTick(); }

}  // namespace genshicraft