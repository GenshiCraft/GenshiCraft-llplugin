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
 * @file playerex.h
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
#include <memory>
#include <string>
#include <third-party/Nlohmann/json.hpp>
#include <vector>

#include "character.h"
#include "menu.h"
#include "sidebar.h"
#include "weapon.h"

namespace genshicraft {

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
  PlayerEx(Player* player);

  /**
   * @brief Destroy the PlayerEx object
   *
   */
  ~PlayerEx();

  /**
   * @brief Consume items
   *
   * @param identifier The identifier of the items
   * @param value The number to consume
   *
   * @exception ExceptionItemsNotEnough The number of the items are fewer than
   * the number to consume.
   */
  void ConsumeItem(std::string identifier, int value);

  /**
   * @brief Consume mora
   *
   * @param value The number to consume
   *
   * @exception ExceptionMoraNotEnough The number of mora is less than the
   * number to consume.
   */
  void ConsumeMora(int value);

  /**
   * @brief Get all characters owned
   *
   * @return The characters
   */
  std::vector<std::shared_ptr<Character>> GetAllCharacters() const;

  /**
   * @brief Get the current character
   *
   * @return The character
   */
  std::shared_ptr<Character> GetCharacter() const;

  /**
   * @brief Get the number of a type of items of the player
   *
   * @param identifier The identifier of the items
   * @return The number
   */
  int GetItemCount(std::string identifier);

  /**
   * @brief Get the Menu handler
   *
   * @return The menu handler
   */
  Menu& GetMenu();

  /**
   * @brief Get the number of mora of the player
   *
   * @return The number
   */
  int GetMoraCount();

  /**
   * @brief Get the Player object
   *
   * @return The Player object
   */
  Player* GetPlayer();

  /**
   * @brief Get the stamina
   *
   * @return The stamina
   */
  int GetStamina() const;

  /**
   * @brief Get the max stamina
   *
   * @return The max stamina
   */
  int GetStaminaMax() const;

  /**
   * @brief Get the Weapon object
   *
   * @return A pointer to the weapon
   */
  std::shared_ptr<Weapon> GetWeapon();

  /**
   * @brief Get the XUID
   *
   * @return The XUID
   */
  const std::string& GetXUID() const;

  /**
   * @brief Give the player items
   *
   * @param identifier The identifier of the items
   * @param value The number of the items
   */
  void GiveItem(const std::string& identifier, int value);

  /**
   * @brief Increase the stamina
   *
   * @param value The stamina to increase
   */
  void IncreaseStamina(int value);

  /**
   * @brief Check if the player is opening a container
   *
   * @return True if the player is opening a container
   */
  bool IsOpeningContainer() const;

  /**
   * @brief Refresh items in the inventory
   *
   */
  void RefreshItems() const;

  /**
   * @brief Set the current character
   *
   * @param no The number of the character in the vector returned by
   * this->GetAllCharacters()
   */
  void SetCharacter(int no);

  /**
   * @brief Set whether the player is opening a container or not
   *
   * @param is_opening_container True if the player is opening a container
   */
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
   * @return A pointer to the PlayerEx object (null std::shared_ptr if not
   * found)
   */
  static std::shared_ptr<PlayerEx> Get(const std::string& xuid);

  /**
   * @brief Get all PlayerEx objects
   *
   * @return A vector containing pointers to all PlayerEx objects
   */
  static std::vector<std::shared_ptr<PlayerEx>>& GetAll();

  /**
   * @brief Load a player
   *
   * @param player A pointer to the player object
   */
  static void LoadPlayer(Player* player);

  /**
   * @brief The function should execute when a player respawns
   *
   * @param player The player
   */
  static void OnPlayerRespawn(Player* player);

  /**
   * @brief This function should execute per tick.
   *
   */
  static void OnTick();

  /**
   * @brief Unload a player
   *
   * @param player A pointer to the player object
   */
  static void UnloadPlayer(Player* player);

 private:
  static const nlohmann::json
      kPlayerDataTemplate;  // the player data template for new players

  std::shared_ptr<Character> character_;  // a pointer to the current character
  std::vector<std::shared_ptr<Character>>
      character_owned_;        // all characters owned
  bool is_opening_container_;  // true if the player is opening a container
  int last_world_level_;       // the world level last tick
  Menu menu_;                  // the menu handler for the player
  Sidebar sidebar_;            // the sidebar handler for the player
  int stamina_;                // the stamina
  int stamina_max_;            // the max value of the stamina
  std::string xuid_;           // the XUID of the player

  static bool is_initialized_;
  static std::vector<std::shared_ptr<PlayerEx>>
      all_playerex_;  // All PlayerEx objects
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_PLAYEREX_H_