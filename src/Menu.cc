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
 * @file Menu.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the Menu class
 * @version 1.0.0
 * @date 2022-07-22
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "Menu.h"

#include <FormUI.h>

#include <MC/Player.hpp>

#include "PlayerEx.h"
#include "Plugin.h"

namespace genshicraft {

Menu::Menu(PlayerEx* playerex) : playerex_(playerex) {
  // empty
}

void Menu::OpenMain() {
  // Title and content
  Form::SimpleForm form("GenshiCraft Menu", "");

  // Options
  form.addButton("Artifacts", "textures/menu/artifacts.bmp")
      .addButton("Character", "textures/menu/character.bmp")
      .addButton("Party Setup", "textures/menu/party_setup.bmp")
      .addButton("Shop", "textures/menu/shop.bmp")
      .addButton("Weapons", "textures/menu/weapons.bmp")
      .addButton("Wish", "textures/menu/wish.bmp");

  form.sendTo(this->playerex_->GetPlayer());
}

}  // namespace genshicraft
