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
 * @file Weapon.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of the Weapon class
 * @version 1.0.0
 * @date 2022-08-04
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_WEAPON_H_
#define GENSHICRAFT_WEAPON_H_

#include <EventAPI.h>

#include <MC/Actor.hpp>
#include <MC/ItemStack.hpp>
#include <MC/Player.hpp>
#include <memory>
#include <string>
#include <vector>

#include "PlayerEx.h"

namespace genshicraft {

/**
 * @brief The Weapon class contains interfaces for weapons.
 *
 */
class Weapon {
 public:
  /**
   * @brief Construct a new Weapon object
   *
   * @param item The weapon item
   * @param playerex The PlayerEx object of the player holding the weapon
   */
  explicit Weapon(ItemStack *item, PlayerEx *playerex);

  /**
   * @brief Get the Ascension Phase
   *
   * @return The Ascension Phase (0 ~ 6)
   */
  int GetAscensionPhase();

  /**
   * @brief Get the Level of the weapon
   *
   * @return The Level of the weapon (1 ~ 70 for 1-Star and 2-Star weapons, 1 ~
   * 90 for others)
   */
  int GetLevel();

  /**
   * @brief Get the Rarity of the weapon
   *
   * @return The Rarity of the weapon (1 ~ 5)
   */
  int GetRarity();

  /**
   * @brief Get the Refinement
   *
   * @return The Refinement (1 ~ 5)
   */
  int GetRefinement();

  /**
   * @brief Get the Weapon EXP
   *
   * @return The Weapon EXP
   */
  int GetWeaponEXP();

  /**
   * @brief Check if the item is a GenshiCraft weapon
   *
   * @param item The item
   * @return True if the item is a GenshiCraft weapon
   */
  static bool CheckIsWeapon(ItemStack *item);

  /**
   * @brief Initialize the weapon system
   *
   */
  static void Init();

 private:
  struct WeaponInfo {
    std::string identifier;
    int rarity;
  };

  const static std::vector<Weapon::WeaponInfo> kWeaponInfoList;

  const static std::vector<int>
      k1StarLevelWeaponEXPList;  // weapon EXP required by each level of 1-Star
                                 // weapons
  const static std::vector<int>
      k2StarLevelWeaponEXPList;  // weapon EXP required by each level of 2-Star
                                 // weapons
  const static std::vector<int>
      k3StarLevelWeaponEXPList;  // weapon EXP required by each level of 3-Star
                                 // weapons
  const static std::vector<int>
      k4StarLevelWeaponEXPList;  // weapon EXP required by each level of 4-Star
                                 // weapons
  const static std::vector<int>
      k5StarLevelWeaponEXPList;  // weapon EXP required by each level of 5-Star
                                 // weapons

  std::string identifier_;  // the Identifier
  int weapon_exp_;          // the Weapon EXP
  int ascension_phase_;     // the Ascension Phase
  int refinement_;          // the Refinement

  static bool is_initialized_;  // the flag indicating if the weapon system is
                                // initialized
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_WEAPON_H_