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
 * @file PlayerEx.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the PlayerEx class
 * @version 1.0.0
 * @date 2022-07-23
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "PlayerEx.h"

#include <EventAPI.h>
#include <Global.h>
#include <KVDBAPI.h>

#include <MC/Level.hpp>
#include <MC/Player.hpp>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "Menu.h"
#include "Plugin.h"
#include "exceptions.h"

namespace genshicraft {

PlayerEx::PlayerEx(Player* player)
    : is_opening_container_(false),
      menu_(Menu(this)),
      xuid_(player->getXuid()) {
  // Ensure that the PlayerEx system is initialized
  if (!PlayerEx::is_initialized_) {
    PlayerEx::Init();
  }

  auto players_db = KVDB::open("plugins/GenshiCraft/db/players");
  logger.info("Player construct");
}

PlayerEx::~PlayerEx() {
  auto players_db = KVDB::open("plugins/GenshiCraft/db/players");
  logger.info("Player destruct");
}

Menu& PlayerEx::GetMenu() { return this->menu_; }

Player* PlayerEx::GetPlayer() { return Global<Level>->getPlayer(this->xuid_); }

const std::string& PlayerEx::GetXUID() const { return this->xuid_; }

bool PlayerEx::IsOpeningContainer() const {
  return this->is_opening_container_;
}

void PlayerEx::SetIsOpeningContainer(bool is_opening_container) {
  this->is_opening_container_ = is_opening_container;
}

void PlayerEx::Init() {
  if (PlayerEx::is_initialized_) {
    return;
  }

  PlayerEx::is_initialized_ = true;
}

PlayerEx* PlayerEx::Get(const std::string& xuid) {
  auto pred = [xuid](const PlayerEx* playerex) {
    return (playerex->GetXUID() == xuid);
  };  // the predicate
  auto iter = std::find_if(PlayerEx::all_playerex_.begin(),
                           PlayerEx::all_playerex_.end(), pred);

  if (iter == PlayerEx::all_playerex_.end()) {  // if not found
    return nullptr;
  }

  return *iter;
}

std::vector<PlayerEx*>& PlayerEx::GetAll() { return PlayerEx::all_playerex_; }

void PlayerEx::LoadPlayer(Player* player) {
  if (PlayerEx::Get(player->getXuid()) ==
      nullptr) {  // to prevent duplicated load
    PlayerEx::all_playerex_.push_back(new PlayerEx(player));
  }
}

void PlayerEx::UnloadPlayer(Player* player) {
  if (PlayerEx::Get(player->getXuid()) !=
      nullptr) {  // to prevent duplicated unload
    auto& all_playerex = PlayerEx::GetAll();
    for (auto it = all_playerex.begin(); it != all_playerex.end(); ++it) {
      if ((*it)->GetXUID() == player->getXuid()) {
        delete (*it);
        all_playerex.erase(it);
        break;
      }
    }
  }
}

bool PlayerEx::is_initialized_ = false;

std::vector<PlayerEx*> PlayerEx::all_playerex_;

}  // namespace genshicraft