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
 * @file KukiShinobu.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of the KukiShinobu class
 * @version 1.0.0
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_CHARACTERS_KUKISHINOBU_H_
#define GENSHICRAFT_CHARACTERS_KUKISHINOBU_H_

#include <memory>

#include "Character.h"
#include "PlayerEx.h"

namespace genshicraft {

class KukiShinobu : public Character {
 public:
  /**
   * @brief Construct a new Kuki Shinobu object
   *
   * @param playerex The PlayerEx object of the owner
   * @param ascension_phase The ascention phase (0 <= x <= 6)
   * @param character_EXP The character EXP (x >= 0)
   * @param constellation The constellation (0 <= x <= 6)
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
              int constellation, int HP, int talent_elemental_burst_level,
              int talent_elemental_skill_level, int talent_normal_attack_level);

  KukiShinobu() = delete;

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
   * @brief Get the stats
   *
   * @return The stats
   */
  struct Character::Stats GetStats() const override;

 private:
  static const int
      kMaxHPBase[7];  // the inferred 0-level max HP of each ascension phase
  static const int kMaxHPDiff;  // the difference of max HP between levels

  static const int
      kATKBase[7];  // the inferred 0-level ATK of each ascension phase
  static const int kATKDiff;  // the difference of ATK between levels

  static const int
      kDEFBase[7];  // the inferred 0-level DEF of each ascension phase
  static const int kDEFDiff;  // the difference of DEF between levels

  static const double
      kMaxHPPercent[7];  // the max HP % attributes of each ascension phase
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_CHARACTERS_KUKISHINOBU_H_