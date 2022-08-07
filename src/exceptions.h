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
 * @file exceptions.h
 * @author Futrime (futrime@outlook.com)
 * @brief Declaration of exception classes
 * @version 1.0.0
 * @date 2022-08-05
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#ifndef GENSHICRAFT_EXCEPTIONS_H_
#define GENSHICRAFT_EXCEPTIONS_H_

#include <stdexcept>

namespace genshicraft {

/**
 * @brief The ExceptionInvalidArgument class represents invalid argument.
 *
 */
class ExceptionInvalidArgument : public std::invalid_argument {
 public:
  ExceptionInvalidArgument()
      : invalid_argument("The argument is invalid."){
            // empty
        };

  using std::invalid_argument::invalid_argument;
};

/**
 * @brief The ExceptionNotAWeapon class represents that the item is not a
 * GenshiCraft item.
 *
 */
class ExceptionNotAWeapon : public ExceptionInvalidArgument {
 public:
  ExceptionNotAWeapon()
      : ExceptionInvalidArgument("The item is not a GenshiCraft weapon.") {
    // empty
  }

  using ExceptionInvalidArgument::ExceptionInvalidArgument;
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_EXCEPTIONS_H_