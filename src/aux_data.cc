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
 * @file aux_data.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the AuxData class
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "aux_data.h"

#include <string>
#include <third-party/Nlohmann/json.hpp>

namespace genshicraft {

void AuxData::Clear() { this->data_.clear(); }

void AuxData::FromString(const std::string& json_string) {
  this->data_ = nlohmann::json::parse(json_string);
}

template <typename T>
T AuxData::Get(const std::string& key) const {
  return this->data_[key].get<T>();
}

void AuxData::Remove(const std::string& key) { this->data_.erase(key); }

template <typename T>
void AuxData::Set(const std::string& key, const T& value) {
  this->data_[key] = value;
}

std::string AuxData::ToString() const { return nlohmann::to_string(this->data_); }

}  // namespace genshicraft