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
 * @file modifier.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the Modifier class
 * @version 1.0.0
 * @date 2022-08-16
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "modifier.h"

#include "exceptions.h"
#include "stats.h"
#include "world.h"

namespace genshicraft {

template <>
Modifier::Modifier(const Stats& value, double expiration)
    : id_(Modifier::GenerateID()),
      expiration_(expiration),
      stats_(value),
      type_(Modifier::Type::kStats) {
  // Empty
}

template <>
Modifier::Modifier(const world::StatusType& value, double expiration)
    : id_(Modifier::GenerateID()),
      expiration_(expiration),
      status_(value),
      type_(Modifier::Type::kStatus) {
  // Empty
}

double Modifier::GetExpiration() const { return this->expiration_; }

int Modifier::GetID() const { return this->id_; }

template <>
Stats Modifier::Get<Stats>() const {
  if (this->type_ != Modifier::Type::kStats) {
    throw ExceptionIncorrectModifierType();
  }

  return this->stats_;
}

template <>
world::StatusType Modifier::Get<world::StatusType>() const {
  if (this->type_ != Modifier::Type::kStatus) {
    throw ExceptionIncorrectModifierType();
  }

  return this->status_;
}

Modifier::Type Modifier::GetType() const { return this->type_; }

int Modifier::GenerateID() {
  ++Modifier::latest_id_;
  return Modifier::latest_id_;
}

}  // namespace genshicraft
