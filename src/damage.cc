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
 * @file Damage.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the Damage class
 * @version 1.0.0
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "damage.h"

#include <algorithm>
#include <random>

#include "character.h"
#include "plugin.h"
#include "stats.h"

namespace genshicraft {

Damage::Damage()
    : amplifier_(1.),
      element_(Damage::ElementType::kPhysical),
      level_(1),
      source_type_(Damage::SourceType::kMob),
      stats_(Stats()),
      victim_element_(Damage::ElementType::kPhysical),
      victim_level_(1) {
  // Empty
}

double Damage::Get() const {
  static std::default_random_engine random_engine;
  static std::uniform_real_distribution<> dist(0, 1);

  // Basic damage
  double damage = this->stats_.GetATK() * this->amplifier_;

  // Damage bonus
  double damage_bonus = 1.;

  switch (this->element_) {
    case Damage::ElementType::kPyro:
      damage_bonus += this->stats_.pyro_DMG_bonus;
      break;

    case Damage::ElementType::kHydro:
      damage_bonus += this->stats_.hydro_DMG_bonus;
      break;

    case Damage::ElementType::kDendro:
      damage_bonus += this->stats_.dendro_DMG_bonus;
      break;

    case Damage::ElementType::kElectro:
      damage_bonus += this->stats_.electro_DMG_bonus;
      break;

    case Damage::ElementType::kAnemo:
      damage_bonus += this->stats_.anemo_DMG_bonus;
      break;

    case Damage::ElementType::kCryo:
      damage_bonus += this->stats_.cryo_DMG_bonus;
      break;

    case Damage::ElementType::kGeo:
      damage_bonus += this->stats_.geo_DMG_bonus;
      break;

    case Damage::ElementType::kPhysical:
      damage_bonus += this->stats_.physical_DMG_bonus;
      break;

    default:
      break;
  }

  damage *= damage_bonus;

  // CRIT
  if (dist(random_engine) < this->stats_.CRIT_rate) {
    damage *= (1. + this->stats_.CRIT_DMG);
  }

  // Elemental reactions
  double reaction_bonus = 1.;

  if (this->element_ == Damage::ElementType::kPyro &&
      this->victim_element_ == Damage::ElementType::kHydro) {
    reaction_bonus = 1.5;
  } else if (this->element_ == Damage::ElementType::kHydro &&
             this->victim_element_ == Damage::ElementType::kPyro) {
    reaction_bonus = 2.;
  } else if (this->element_ == Damage::ElementType::kPyro &&
             this->victim_element_ == Damage::ElementType::kCryo) {
    reaction_bonus = 2.;
  } else if (this->element_ == Damage::ElementType::kCryo &&
             this->victim_element_ == Damage::ElementType::kPyro) {
    reaction_bonus = 1.5;
  }

  if (reaction_bonus > 1.0001) {
    reaction_bonus *= 1. + (2.78 * this->stats_.elemental_mastery /
                            (this->stats_.elemental_mastery + 1400));
  }

  damage *= reaction_bonus;

  // DEF
  damage *= ((this->level_ + 100) * 5.) /
            ((this->level_ + 100) * 5. + this->victim_stats_.GetDEF());

  // RES
  double RES = 0.;
  switch (this->element_) {
    case Damage::ElementType::kPyro:
      RES += this->victim_stats_.pyro_RES;
      break;

    case Damage::ElementType::kHydro:
      RES += this->victim_stats_.hydro_RES;
      break;

    case Damage::ElementType::kDendro:
      RES += this->victim_stats_.dendro_RES;
      break;

    case Damage::ElementType::kElectro:
      RES += this->victim_stats_.electro_RES;
      break;

    case Damage::ElementType::kAnemo:
      RES += this->victim_stats_.anemo_RES;
      break;

    case Damage::ElementType::kCryo:
      RES += this->victim_stats_.cryo_RES;
      break;

    case Damage::ElementType::kGeo:
      RES += this->victim_stats_.geo_RES;
      break;

    case Damage::ElementType::kPhysical:
      RES += this->victim_stats_.physical_RES;
      break;

    default:
      break;
  }

  if (RES < 0.) {
    damage *= 1. - RES / 2;
  } else if (RES < 0.75) {
    damage *= 1. - RES;
  } else {
    damage *= 1. / (1. + RES * 4);
  }

  damage = std::max(damage, 0.);  // the damage must not be negative

  return damage;
}

Damage::ElementType Damage::GetElementType() const { return this->element_; }

Damage::ElementalReactionType Damage::GetElementalReactionType() const {
  if ((this->element_ == ElementType::kDendro &&
       this->victim_element_ == ElementType::kHydro) ||
      (this->victim_element_ == ElementType::kDendro &&
       this->element_ == ElementType::kHydro)) {
    return Damage::ElementalReactionType::kBloom;

  } else if ((this->element_ == ElementType::kDendro &&
              this->victim_element_ == ElementType::kPyro) ||
             (this->victim_element_ == ElementType::kDendro &&
              this->element_ == ElementType::kPyro)) {
    return Damage::ElementalReactionType::kBurning;

  } else if ((this->element_ == ElementType::kDendro &&
              this->victim_element_ == ElementType::kElectro) ||
             (this->victim_element_ == ElementType::kDendro &&
              this->element_ == ElementType::kElectro)) {
    return Damage::ElementalReactionType::kCatalyze;

  } else if ((this->element_ == ElementType::kGeo &&
              (this->victim_element_ == ElementType::kCryo ||
               this->victim_element_ == ElementType::kElectro ||
               this->victim_element_ == ElementType::kHydro ||
               this->victim_element_ == ElementType::kPyro)) ||
             ((this->victim_element_ == ElementType::kCryo ||
               this->victim_element_ == ElementType::kElectro ||
               this->victim_element_ == ElementType::kHydro ||
               this->victim_element_ == ElementType::kPyro) &&
              this->victim_element_ == ElementType::kGeo)) {
    return Damage::ElementalReactionType::kCrystallize;

  } else if ((this->element_ == ElementType::kElectro &&
              this->victim_element_ == ElementType::kHydro) ||
             (this->victim_element_ == ElementType::kElectro &&
              this->element_ == ElementType::kHydro)) {
    return Damage::ElementalReactionType::kElectroCharged;

  } else if ((this->element_ == ElementType::kCryo &&
              this->victim_element_ == ElementType::kHydro) ||
             (this->victim_element_ == ElementType::kCryo &&
              this->element_ == ElementType::kHydro)) {
    return Damage::ElementalReactionType::kFrozen;

  } else if ((this->element_ == ElementType::kCryo &&
              this->victim_element_ == ElementType::kPyro) ||
             (this->victim_element_ == ElementType::kCryo &&
              this->element_ == ElementType::kPyro)) {
    return Damage::ElementalReactionType::kMelt;

  } else if ((this->element_ == ElementType::kElectro &&
              this->victim_element_ == ElementType::kPyro) ||
             (this->victim_element_ == ElementType::kElectro &&
              this->element_ == ElementType::kPyro)) {
    return Damage::ElementalReactionType::kOverloaded;

  } else if ((this->element_ == ElementType::kCryo &&
              this->victim_element_ == ElementType::kElectro) ||
             (this->victim_element_ == ElementType::kCryo &&
              this->element_ == ElementType::kElectro)) {
    return Damage::ElementalReactionType::kSuperconduct;

  } else if ((this->element_ == ElementType::kAnemo &&
              (this->victim_element_ == ElementType::kCryo ||
               this->victim_element_ == ElementType::kElectro ||
               this->victim_element_ == ElementType::kHydro ||
               this->victim_element_ == ElementType::kPyro)) ||
             ((this->victim_element_ == ElementType::kCryo ||
               this->victim_element_ == ElementType::kElectro ||
               this->victim_element_ == ElementType::kHydro ||
               this->victim_element_ == ElementType::kPyro) &&
              this->victim_element_ == ElementType::kAnemo)) {
    return Damage::ElementalReactionType::kSwirl;

  } else {
    return Damage::ElementalReactionType::kNone;
  }
}

Damage::SourceType Damage::GetSourceType() const { return this->source_type_; }

void Damage::SetAmplifier(double amplifier) { this->amplifier_ = amplifier; }

void Damage::SetElementType(const Damage::ElementType& element) {
  this->element_ = element;
}

void Damage::SetLevel(int level) { this->level_ = level; }

void Damage::SetSourceType(const Damage::SourceType& source_type) {
  this->source_type_ = source_type;
};

void Damage::SetStats(const Stats& stats) { this->stats_ = stats; }

void Damage::SetVictimElementType(const Damage::ElementType& element) {
  this->victim_element_ = element;
}

void Damage::SetVictimLevel(int level) { this->victim_level_ = level; }

void Damage::SetVictimStats(const Stats& stats) { this->victim_stats_ = stats; }

}  // namespace genshicraft
