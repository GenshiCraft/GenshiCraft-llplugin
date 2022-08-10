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
 * @file Sidebar.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the Sidebar class
 * @version 1.0.0
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "Sidebar.h"

#include <MC/Player.hpp>
#include <MC/Types.hpp>
#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include "PlayerEx.h"
#include "Plugin.h"

namespace genshicraft {

Sidebar::Sidebar(PlayerEx* playerex) : playerex_(playerex) {
  // Empty
}

void Sidebar::Refresh() {
  std::vector<std::pair<std::string, int>> content;

  // Stamina
  auto stamina_progress = static_cast<double>(this->playerex_->GetStamina()) /
                          this->playerex_->GetStaminaMax();
  content.push_back(
      {"Stamina " + Sidebar::GenerateProgressBar(
                        stamina_progress, this->playerex_->GetStaminaMax() / 10,
                        (stamina_progress < 0.3) ? "§c" : "§e"),
       0});

  // HP
  auto HP_progress =
      static_cast<double>(this->playerex_->GetCharacter()->GetHP()) /
      this->playerex_->GetCharacter()->GetStats().max_HP;
  content.push_back(
      {"HP " + Sidebar::GenerateProgressBar(
                   HP_progress, 24, (HP_progress < 0.3) ? "§c" : "§a"),
       1});

  auto player = this->playerex_->GetPlayer();
  player->removeSidebar();
  player->setSidebar(this->playerex_->GetCharacter()->GetName(), content,
                     ObjectiveSortOrder::Ascending);
}

std::string Sidebar::GenerateProgressBar(double value, int steps,
                                         std::string formatting_codes) {
  auto filled_steps = static_cast<int>(value * steps);

  std::string progress_bar = formatting_codes;
  for (int i = 0; i < steps; ++i) {
    if (i < filled_steps) {
      progress_bar += "|";
    } else {
      progress_bar += ".";
    }
  }

  return progress_bar;
}

}  // namespace genshicraft
