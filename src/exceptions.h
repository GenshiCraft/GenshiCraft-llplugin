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
#include <string>

namespace genshicraft {

/**
 * @brief The Exception class is the base class for all exceptions.
 *
 */
class Exception : public std::exception {
 public:
  Exception(const std::string& what_arg) : what_arg_(what_arg) {}

  virtual const char* what() const noexcept { return this->what_arg_.c_str(); }

 private:
  std::string what_arg_;
};

/**
 * @brief The ExceptionCharacter class is the base class for exceptions related
 * to the Character class.
 *
 */
class ExceptionCharacter : public Exception {
 public:
  using Exception::Exception;
};

/**
 * @brief The ExceptionInvalidCharacterData class represents that the character
 * data is invalid.
 *
 */
class ExceptionInvalidCharacterData : public ExceptionCharacter {
 public:
  ExceptionInvalidCharacterData()
      : ExceptionCharacter("The character data is invalid.") {}

  using ExceptionCharacter::ExceptionCharacter;
};

/**
 * @brief The ExceptionNotACharacter class represents that the name does not
 * match any character.
 *
 */
class ExceptionNotACharacter : public ExceptionCharacter {
 public:
  ExceptionNotACharacter()
      : ExceptionCharacter("The name does not match any character.") {}

  using ExceptionCharacter::ExceptionCharacter;
};

/**
 * @brief The ExceptionWeapon class is the base class for exceptions related to
 * the Weapon class.
 *
 */
class ExceptionWeapon : public Exception {
 public:
  using Exception::Exception;
};

/**
 * @brief The ExceptionNotAWeapon class represents that the item is not a
 * GenshiCraft item.
 *
 */
class ExceptionNotAWeapon : public ExceptionWeapon {
 public:
  ExceptionNotAWeapon()
      : ExceptionWeapon("The item is not a GenshiCraft weapon.") {}

  using ExceptionWeapon::ExceptionWeapon;
};

}  // namespace genshicraft

#endif  // GENSHICRAFT_EXCEPTIONS_H_