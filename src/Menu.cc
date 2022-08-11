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
 * @file menu.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the Menu class
 * @version 1.0.0
 * @date 2022-07-22
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "menu.h"

#include <FormUI.h>

#include <MC/Player.hpp>
#include <memory>
#include <string>

#include "playerex.h"
#include "plugin.h"
#include "weapon.h"

namespace genshicraft {

Menu::Menu(PlayerEx* playerex) : playerex_(playerex) {
  // empty
}

void Menu::OpenMain() {
  // Title and content
  Form::SimpleForm form("GenshiCraft Menu", "");

  // Options
  form = form.addButton("Artifacts", "textures/menu/artifacts.bmp");

  form = form.addButton("Character", "textures/menu/character.bmp");

  form = form.addButton("Party Setup", "textures/menu/party_setup.bmp");

  form = form.addButton("Shop", "textures/menu/shop.bmp");

  form = form.addButton("Weapon", "textures/menu/weapons.bmp",
                        [this](Player* player) { this->OpenWeapon(); });

  form = form.addButton("Wish", "textures/menu/wish.bmp");

  form.sendTo(this->playerex_->GetPlayer());
}

void Menu::OpenWeapon() {
  // Check if the player is holding a GenshiCraft weapon
  if (!this->playerex_->GetWeapon()) {
    this->OpenMain();
    return;
  }

  auto weapon = this->playerex_->GetWeapon();

  // Title and content
  Form::SimpleForm form(
      "Weapon / " + std::string(this->playerex_->GetWeapon()->GetName()),
      "Test\nTest");

  // Options
  if (weapon->kAcensionPhaseMaxLevelList[weapon->GetAscensionPhase()] ==
      weapon->GetLevel()) {
    if ((weapon->GetRarity() >= 3 && weapon->GetLevel() != 90) ||
        (weapon->GetRarity() <= 2 && weapon->GetLevel() != 70)) {
      form = form.addButton(
          "Ascend", "", [this](Player* player) { this->OpenWeaponAscend(); });
    }
  } else {
    form = form.addButton(
        "Enhance", "", [this](Player* player) { this->OpenWeaponEnhance(); });
  }

  if (weapon->GetRarity() >= 3 && weapon->GetRefinement() < 5) {
    form = form.addButton("Refine", "",
                          [this](Player* player) { this->OpenWeaponRefine(); });
  }

  form.sendTo(this->playerex_->GetPlayer());
}

void Menu::OpenWeaponAscend() {
  // Check if the player is holding a GenshiCraft weapon
  if (!this->playerex_->GetWeapon()) {
    this->OpenMain();
    return;
  }
}

void Menu::OpenWeaponEnhance() {
  // Check if the player is holding a GenshiCraft weapon
  if (!this->playerex_->GetWeapon()) {
    this->OpenMain();
    return;
  }
}

void Menu::OpenWeaponRefine() {
  // Check if the player is holding a GenshiCraft weapon
  if (!this->playerex_->GetWeapon()) {
    this->OpenMain();
    return;
  }
}
}  // namespace genshicraft
