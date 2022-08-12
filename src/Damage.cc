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
 * @file Damage.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the Damage class
 * @version 1.0.0
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "damage.h"

#include <random>

#include "character.h"
#include "plugin.h"

namespace genshicraft {

Damage::Damage() : amplifier_(1.) {
  // Empty
}

double Damage::Get() const {
  static std::default_random_engine random_engine;
  static std::uniform_real_distribution<> dist(0, 1);

  double damage = this->stats_.GetATK();

  damage *= this->amplifier_;

  if (dist(random_engine) < this->stats_.CRIT_rate) {
    damage *= (1. + this->stats_.CRIT_DMG);
  }

  return damage;
}

void Damage::SetAmplifier(double amplifier) { this->amplifier_ = amplifier; }

void Damage::SetStats(const Character::Stats& stats) { this->stats_ = stats; }

}  // namespace genshicraft
