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
 * @file command.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the Command class
 * @version 1.0.0
 * @date 2022-07-22
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "command.h"

#include <DynamicCommandAPI.h>

#include <MC/CommandOrigin.hpp>
#include <MC/CommandOutput.hpp>
#include <MC/ServerPlayer.hpp>
#include <unordered_map>

#include "menu.h"
#include "playerex.h"

namespace genshicraft {

void Command::Init() {
  DynamicCommand::setup(
      "menu", "Open GenshiCraft menu", {}, {},
      {
          {},
      },
      [](DynamicCommand const& command, CommandOrigin const& origin,
         CommandOutput& output,
         std::unordered_map<std::string, DynamicCommand::Result>& results) {
        auto xuid = origin.getPlayer()->getXuid();
        auto playerex = PlayerEx::Get(xuid);
        playerex->GetMenu().OpenMain();
      });
}

}  // namespace genshicraft
