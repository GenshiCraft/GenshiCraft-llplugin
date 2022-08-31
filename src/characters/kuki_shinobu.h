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
 * @file kuki_shinobu.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of the KukiShinobu class
 * @version 1.0.0
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_CHARACTERS_KUKI_SHINOBU_H_
#define GENSHICRAFT_CHARACTERS_KUKI_SHINOBU_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "character.h"
#include "playerex.h"
#include "stats.h"

namespace genshicraft {

/**
 * @brief The KukiShinobu class contains interfaces for Kuki Shinobu.
 *
 */
class KukiShinobu final : public Character {
 public:
  /**
   * @brief Construct a new Kuki Shinobu object
   *
   * @param playerex The PlayerEx object of the owner
   * @param ascension_phase The ascention phase (0 <= x <= 6)
   * @param character_EXP The character EXP (x >= 0)
   * @param constellation The constellation (0 <= x <= 6)
   * @param energy The energy (x >= 0)
   * @param HP The HP (x >= 0)
   * @param talent_elemental_burst_level The level of elemental burst (1 <= x <=
   * 10)
   * @param talent_elemental_skill_level The level of elemental skill (1 <= x <=
   * 10)
   * @param talent_normal_attakck_level The level of normal attack (1 <= X <=
   * 10)
   *
   * @exception ExceptionInvalidCharacterData The character data is invalid.
   */
  KukiShinobu(PlayerEx* playerex, int ascension_phase, int character_EXP,
              int constellation, int energy, int HP,
              int talent_elemental_burst_level,
              int talent_elemental_skill_level, int talent_normal_attack_level);

  KukiShinobu() = delete;

  /**
   * @brief Get the ascension materials
   *
   * @return The names and the numbers of the ascension materials
   */
  std::map<std::string, int> GetAscensionMaterials() const override;

  /**
   * @brief Perform an attack and return its damage
   *
   * @param attack_type The attack type
   * @return The damage
   */
  Damage GetAttackDamage(const world::AttackType& attack_type) override;

  /**
   * @brief Get the base stats
   *
   * @return The stats
   */
  Stats GetBaseStats() const override;

  /**
   * @brief Get the CD of the talent
   *
   * @param talent The talent
   * @return The CD
   *
   * @note For talents without CD or nonexistent, this method will always return
   * 0.
   */
  double GetCDMax(const world::TalentType& talent) const override;

  /**
   * @brief Get the max energy
   *
   * @return 60
   */
  int GetEnergyMax() const override;

  /**
   * @brief Get the name
   *
   * @return "Kuki Shinobu"
   */
  std::string GetName() const override;

  /**
   * @brief Get the rarity
   *
   * @return 4
   */
  int GetRarity() const override;

  /**
   * @brief Get the talent level-up materials
   *
   * @return The names and the numbers of the materials
   */
  std::map<std::string, int> GetTalentLevelUpMaterials(
      const world::TalentType& talent) const override;

  /**
   * @brief Check if the character is holding a weapon
   *
   * @return True if the character is holding a weapon
   */
  bool HasWeapon() const override;

 private:
  static const std::vector<std::map<std::string, int>>
      kAscensionMaterialsList;  // the ascension materials

  static const std::vector<int>
      kStatsATKBase;  // the inferred 0-level ATK of each ascension phase
  static const int kStatsATKDiff;  // the difference of ATK between levels

  static const std::vector<int>
      kStatsDEFBase;  // the inferred 0-level DEF of each ascension phase
  static const int kStatsDEFDiff;  // the difference of DEF between levels

  static const std::vector<int> kStatsMaxHPBase;  // the inferred 0-level max HP
                                                  // of each ascension phase
  static const int kStatsMaxHPDiff;  // the difference of max HP between levels

  static const std::vector<double>
      kStatsMaxHPPercent;  // the max HP % attributes of each ascension phase

  static const std::vector<std::vector<double>>
      kTalentNormalAttackChargedAttackDMG;

  static const std::vector<std::vector<double>> kTalentNormalAttackHitDMG;

  static const std::vector<double> kTalentNormalAttackPlungeHighDMG;

  static const std::vector<double> kTalentNormalAttackPlungeLowDMG;

  static const std::vector<double> kTalentNormalAttackPlungeRegularDMG;

  static const std::vector<std::map<std::string, int>>
      kTalentLevelUpMaterialList;  // the talent level-up materials
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_CHARACTERS_KUKI_SHINOBU_H_