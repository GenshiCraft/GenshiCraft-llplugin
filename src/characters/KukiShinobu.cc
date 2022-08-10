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
 * @file KukiShinobu.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the KukiShinobu class
 * @version 1.0.0
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "KukiShinobu.h"

#include <memory>
#include <string>

#include "Character.h"
#include "PlayerEx.h"
#include "Plugin.h"
#include "Weapon.h"
#include "exceptions.h"

namespace genshicraft {

KukiShinobu::KukiShinobu(PlayerEx* playerex, int ascension_phase,
                         int character_EXP, int constellation, int HP,
                         int talent_elemental_burst_level,
                         int talent_elemental_skill_level,
                         int talent_normal_attack_level)
    : Character(playerex, ascension_phase, character_EXP, constellation, HP,
                talent_elemental_burst_level, talent_elemental_skill_level,
                talent_normal_attack_level) {
  if (HP > this->GetStats().max_HP) {
    throw ExceptionInvalidCharacterData();
  }
}

std::string KukiShinobu::GetName() const { return "Kuki Shinobu"; }

int KukiShinobu::GetRarity() const { return 4; }

struct Character::Stats KukiShinobu::GetStats() const {
  struct Character::Stats stats;
  stats.max_HP_base = KukiShinobu::kMaxHPBase[this->GetAscensionPhase()] +
                      KukiShinobu::kMaxHPDiff * this->GetLevel();
  stats.ATK_base = KukiShinobu::kATKBase[this->GetAscensionPhase()] +
                   KukiShinobu::kATKDiff * this->GetLevel();
  stats.DEF_base = KukiShinobu::kDEFBase[this->GetAscensionPhase()] +
                   KukiShinobu::kDEFDiff * this->GetLevel();

  if (this->GetPlayerEx()
          ->GetWeapon()) {  // if the player is holding a GenshiCraft weapon
    stats.max_HP_base += this->GetPlayerEx()->GetWeapon()->GetATK();
  }

  stats.max_HP = static_cast<int>(
      stats.max_HP_base *
      (1 + KukiShinobu::kMaxHPPercent[this->GetAscensionPhase()]));
  stats.ATK = stats.ATK_base;
  stats.DEF = stats.DEF_base;

  stats.max_stamina = this->GetPlayerEx()->GetStaminaMax();

  if (this->GetPlayerEx()
          ->GetWeapon()) {  // if the player is holding a GenshiCraft weapon
    stats = this->GetPlayerEx()->GetWeapon()->ApplyModifiers(stats);
  }

  return stats;
}

const int KukiShinobu::kMaxHPBase[7] = {945,  1717, 2265, 2995,
                                        3543, 4090, 4638};

const int KukiShinobu::kMaxHPDiff = 85;

const int KukiShinobu::kATKBase[7] = {17, 39, 58, 75, 90, 104, 118};

const int KukiShinobu::kATKDiff = 1;

const int KukiShinobu::kDEFBase[7] = {58, 109, 146, 193, 228, 263, 299};

const int KukiShinobu::kDEFDiff = 5;

const double KukiShinobu::kMaxHPPercent[7] = {0,    0,    0.06, 0.12,
                                              0.12, 0.18, 0.24};

}  // namespace genshicraft
