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
 * @file modifier.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of the Modifier class
 * @version 1.0.0
 * @date 2022-08-16
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_MODIFIER_H_
#define GENSHICRAFT_MODIFIER_H_

#include "stats.h"
#include "world.h"

namespace genshicraft {

/**
 * @brief The Modifier class represents the modification to character attributes
 * and effects.
 *
 */
class Modifier {
 public:
  /**
   * @brief The modifier type
   *
   */
  enum class Type { kStats = 0, kStatus };

  Modifier() = delete;

  /**
   * @brief Construct a new Modifier
   *
   * @param value The modifier value
   * @param expiration The expiration
   */
  template <typename T>
  Modifier(const T& value, double expiration);

  /**
   * @brief Get the expired clock
   *
   * @return The expired clock
   */
  double GetExpiration() const;

  /**
   * @brief Get the the modifier ID
   *
   * @return The modifier ID
   */
  int GetID() const;

  /**
   * @brief Get the modifier value
   *
   * @tparam T The type to get
   * @return The modifier value as the type
   * @exception ExceptionIncorrectModifierType The modifier cannot be got as the
   * type
   */
  template <typename T>
  T Get() const;

  /**
   * @brief Get the type of the modifier
   *
   * @return The type
   */
  Type GetType() const;

 private:
  /**
   * @brief Generate an modifier ID
   *
   * @return The ID
   */
  static int GenerateID();

  int id_;             // the ID
  double expiration_;  // the clock when the modifier expired
  Type type_;          // the type

  // The members below are not always in use
  Stats stats_;               // the stats
  world::StatusType status_;  // the status

  inline static int latest_id_ = 0;  // the latest used ID
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_MODIFIER_H_