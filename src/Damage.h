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
 * @file damage.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of the damage class
 * @version 1.0.0
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_DAMAGE_H_
#define GENSHICRAFT_DAMAGE_H_

#include "character.h"
#include "stats.h"

namespace genshicraft {

/**
 * @brief The Damage class is used for damage calculation.
 *
 */
class Damage {
 public:
  /**
   * @brief The element type
   *
   */
  enum class ElementType {
    kAnemo = 0,
    kCryo,
    kDendro,
    kElectro,
    kGeo,
    kHydro,
    kPhysical,
    kPyro
  };

  /**
   * @brief The elemental reaction type
   *
   */
  enum class ElementalReactionType {
    kNone = 0,
    kBloom,
    kBurning,
    kCatalyze,
    kCrystallize,
    kElectroCharged,
    kFrozen,
    kMelt,
    kOverloaded,
    kSuperconduct,
    kSwirl
  };

  /**
   * @brief The source type
   *
   */
  enum class SourceType { kEnvironment = 0, kMob };

  /**
   * @brief Construct a new Damage object
   *
   */
  Damage();

  /**
   * @brief Get the damage value
   *
   * @return The damage value
   */
  double Get() const;

  /**
   * @brief Get the element type of the damage
   *
   * @return The element type
   */
  ElementType GetElementType() const;

  /**
   * @brief Get the elemental reaction type
   *
   * @return The elemental reaction type
   */
  ElementalReactionType GetElementalReactionType() const;

  /**
   * @brief Get the source type
   *
   * @return The source type
   */
  SourceType GetSourceType() const;

  /**
   * @brief Set the skill amplifier of the attacker
   *
   * @param amplifier The amplifier
   */
  void SetAmplifier(double amplifier);

  /**
   * @brief Set the element type of the attack
   *
   * @param element The element type
   */
  void SetElementType(const ElementType& element);

  /**
   * @brief Set the level of the attacker
   *
   * @param level The level
   */
  void SetLevel(int level);

  /**
   * @brief Set the source type
   *
   * @param source_type The source type
   */
  void SetSourceType(const SourceType& source_type);

  /**
   * @brief Set the stats of the attacker
   *
   * @param stats The stats
   */
  void SetStats(const Stats& stats);

  /**
   * @brief Set the element attached to the victim
   *
   * @param element The element type
   */
  void SetVictimElementType(const ElementType& element);

  /**
   * @brief Set the level of the victim
   *
   * @param level The level
   */
  void SetVictimLevel(int level);

  /**
   * @brief Set the stats of the victim
   *
   * @param stats The stats
   */
  void SetVictimStats(const Stats& stats);

 private:
  double amplifier_;
  ElementType element_;
  int level_;  // the level of the attacker
  SourceType source_type_;
  Stats stats_;
  ElementType victim_element_;
  int victim_level_;  // the level of the victim
  Stats victim_stats_;
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_DAMAGE_H_