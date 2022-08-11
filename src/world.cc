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
 * @file world.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of global world interfaces and objects
 * @version 1.0.0
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "world.h"

#include <MC/Dimension.hpp>
#include <MC/Vec3.hpp>

namespace genshicraft {

namespace world {

int GetWorldLevel(const Vec3& position, const Dimension& dimension) {
  static const double kWorldLevelMinDistanceList[9] = {
      0, 0, 1024, 2048, 4096, 8192, 16384, 32768, 65536};

  if (dimension.getDimensionId() == 2) {  // the End
    return 8;
  }

  auto distance = position.x * position.x + position.z * position.z;

  if (dimension.getDimensionId() == 1) {  // the Nether
    distance *=
        8;  // the distance in the Nether is 8 times of that in the Main World
  }

  int world_level = 1;
  for (int i = 1; i <= 8; ++i) {
    if (distance > kWorldLevelMinDistanceList[i]) {
      world_level = i;
    }
  }

  return world_level;
}

}  // namespace world

}  // namespace genshicraft