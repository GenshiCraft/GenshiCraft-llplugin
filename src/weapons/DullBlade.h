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
 * @file DullBlade.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of the DullBlade class
 * @version 1.0.0
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_WEAPONS_DULLBLADE_H_
#define GENSHICRAFT_WEAPONS_DULLBLADE_H_

#include <MC/ItemStack.hpp>
#include <memory>

#include "Character.h"
#include "PlayerEx.h"
#include "Weapon.h"

namespace genshicraft {

class DullBlade : public Weapon {
 public:
  /**
   * @brief Construct a new Dull Blade object
   *
   * @param item The weapon item
   * @param playerex The PlayerEx object of the holder
   *
   * @exception ExceptionNotAWeapon The item is not a GenshiCraft weapon
   */
  DullBlade(ItemStack* item, PlayerEx* playerex);

  DullBlade() = delete;

  /**
   * @brief Apply modifiers to character stats
   *
   * @param stats The character stats
   */
  struct Character::Stats ApplyModifiers(
      const struct Character::Stats& stats) const override;

  /**
   * @brief Get the ATK
   *
   * @return The ATK
   */
  int GetATK() const override;

  /**
   * @brief Get the rarity of the weapon
   *
   * @return 1
   */
  int GetRarity() const override;

  /**
   * @brief Get the weapon type
   *
   * @return Weapon::Type::kSword
   */
  virtual Weapon::Type GetType() const override;

 private:
  static const int
      kATKBase[5];  // the inferred 0-level ATK of each ascension phase

  static const int kATKDiff;  // the difference of ATK between levels
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_WEAPONS_DULLBLADE_H_