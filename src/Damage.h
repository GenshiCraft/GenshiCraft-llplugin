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
 * @file Damage.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of the Damage class
 * @version 1.0.0
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_DAMAGE_H_
#define GENSHICRAFT_DAMAGE_H_

#include "Character.h"

namespace genshicraft {

/**
 * @brief The Damage class is used for damage calculation.
 *
 */
class Damage {
 public:
  /**
   * @brief Construct a new Damage object
   *
   */
  Damage();

  /**
   * @brief Get the damage value
   *
   * @return The damage value
   */
  double Get() const;

  /**
   * @brief Set the amplifier
   *
   * @param amplifier The amplifier
   */
  void SetAmplifier(double amplifier);

  /**
   * @brief Set the stats
   *
   * @param stats The stats
   */
  void SetStats(const Character::Stats& stats);

 private:
  double amplifier_;
  Character::Stats stats_;
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_DAMAGE_H_