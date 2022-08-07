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
 * @file PlayerEx.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of the PlayerEx class
 * @version 1.0.0
 * @date 2022-07-23
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_PLAYEREX_H_
#define GENSHICRAFT_PLAYEREX_H_

#include <MC/Player.hpp>
#include <string>
#include <vector>

#include "Menu.h"

namespace genshicraft {

class Menu;

/**
 * @brief The PlayerEx class contains extended interfaces for players.
 *
 */
class PlayerEx {
 public:
  /**
   * @brief Construct a new PlayerEx object
   *
   * @param player A pointer to the player object
   */
  explicit PlayerEx(Player* player);

  /**
   * @brief Destroy the PlayerEx object
   * 
   */
  ~PlayerEx();

  /**
   * @brief Get the Menu handler
   *
   * @return The menu handler
   */
  Menu& GetMenu();

  /**
   * @brief Get the Player object
   *
   * @return The Player object
   */
  Player* GetPlayer();

  /**
   * @brief Get the XUID
   *
   * @return The XUID
   */
  const std::string& GetXUID() const;

  bool IsOpeningContainer() const;

  void SetIsOpeningContainer(bool is_opening_container);

  /**
   * @brief Initialize the player system
   *
   */
  static void Init();

  /**
   * @brief Get a PlayerEx object by XUID
   *
   * @param uuid The UUID of the player
   * @return A pointer to the PlayerEx object (nullptr if not found)
   */
  static PlayerEx* Get(const std::string& xuid);

  /**
   * @brief Get all PlayerEx objects
   *
   * @return A vector containing pointers to all PlayerEx objects
   */
  static std::vector<PlayerEx*>& GetAll();

  /**
   * @brief Load a player
   *
   * @param player A pointer to the player object
   */
  static void LoadPlayer(Player* player);

  /**
   * @brief Unload a player
   *
   * @param player A pointer to the player object
   */
  static void UnloadPlayer(Player* player);

 private:
  bool is_opening_container_;
  Menu menu_;         // The menu handler for the player
  std::string xuid_;  // The XUID of the player

  static bool is_initialized_;
  static std::vector<PlayerEx*> all_playerex_;  // All PlayerEx objects
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_PLAYEREX_H_