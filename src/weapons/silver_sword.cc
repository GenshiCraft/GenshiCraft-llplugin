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
 * @file silver_sword.h
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the SilverSword class
 * @version 1.0.0
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "silver_sword.h"

#include <MC/ItemStack.hpp>
#include <string>

#include "character.h"
#include "exceptions.h"
#include "playerex.h"
#include "weapon.h"

namespace genshicraft {

SilverSword::SilverSword(ItemStack* item, PlayerEx* playerex)
    : Weapon(item, playerex) {
  // Check if the item is a GenshiCraft item
  if (!Weapon::CheckIsWeapon(item)) {
    throw ExceptionNotAWeapon();
  }

  this->ApplyLore(item, playerex);
}

struct Character::Stats SilverSword::ApplyModifiers(
    const struct Character::Stats& stats) const {
  return stats;
}

int SilverSword::GetATK() const {
  return (SilverSword::kATKBase[this->GetAscensionPhase()] +
          SilverSword::kATKDiff * this->GetLevel());
}

std::string SilverSword::GetName() const { return "Silver Sword"; }

int SilverSword::GetRarity() const { return 2; }

Weapon::Type SilverSword::GetType() const { return Weapon::Type::kSword; }

const int SilverSword::kATKBase[5] = {31, 51, 71, 86, 100};

const int SilverSword::kATKDiff = 2;

}  // namespace genshicraft