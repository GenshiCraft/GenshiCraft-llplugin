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
 * @file aux_data.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of the AuxData class
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_AUX_DATA_H_
#define GENSHICRAFT_AUX_DATA_H_

#include <string>
#include <Nlohmann/json.hpp>

namespace genshicraft {

class AuxData {
 public:
  /**
   * @brief Clear all KV pairs
   * 
   */
  void Clear();

  /**
   * @brief Import the data from a JSON string
   * 
   * @param json_string The JSON string
   */
  void FromString(const std::string& json_string);

  /**
   * @brief Get the value of a KV pair
   *
   * @tparam T The type of the value
   * @param key The key
   */
  template <typename T>
  T Get(const std::string& key) const;

  /**
   * @brief Remove a KV pair
   *
   * @param key The key
   */
  void Remove(const std::string& key);

  /**
   * @brief Set a KV pair. If the KV pair does not exist, a new KV pair will be
   * created.
   *
   * @tparam T The type of the value
   * @param key The key
   * @param value The value
   */
  template <typename T>
  void Set(const std::string& key, const T& value);

  /**
   * @brief Export the data to a JSON string
   * 
   * @return The JSON string
   */
  std::string ToString() const;

 private:
  nlohmann::json data_;
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_AUX_DATA_H_