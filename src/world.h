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
 * @file world.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of world interfaces and objects
 * @version 1.0.0
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_WORLD_H_
#define GENSHICRAFT_WORLD_H_

#include <MC/Dimension.hpp>
#include <MC/Vec3.hpp>

namespace genshicraft {

namespace world {

/**
 * @brief Get the world level of the position
 * 
 * @param position The position
 * @param dimension The dimension
 * @return The world level (1 <= x <= 9)
 */
int GetWorldLevel(const Vec3& position, const Dimension& dimension);

/**
 * @brief Get the multiple of enemy max HP relative to level 1
 * 
 * @param level The level
 * @return The enemy max HP multiple
 */
double GetEnemyMaxHPMultiplier(int level);

/**
 * @brief Get the multiple of enemy ATK relative to level 1
 * 
 * @param level The level
 * @return The enemy ATK multiple
 */
double GetEnemyATKMultiplier(int level);

}  // namespace world

}  // namespace genshicraft

#endif