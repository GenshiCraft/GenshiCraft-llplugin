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
 * @file Character.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the Character class
 * @version 1.0.0
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "Character.h"

#include <algorithm>
#include <memory>
#include <string>

#include "PlayerEx.h"
#include "Plugin.h"
#include "characters/KukiShinobu.h"
#include "exceptions.h"

namespace genshicraft {

int Character::GetAscensionPhase() const { return this->ascension_phase_; }

int Character::GetCharacterEXP() const { return this->character_EXP_; }

int Character::GetConstellation() const { return this->constellation_; }

int Character::GetHP() const { return this->HP_; }

int Character::GetLevel() const {
  int level = 1;
  for (int i = 1; i <= 90; ++i) {
    if (this->character_EXP_ >= Character::kLevelMinCharacterEXPList[i]) {
      level = i;
    }
  }
  level = std::min(
      level, Character::kAcensionPhaseMaxLevelList[this->ascension_phase_]);
  return level;
}

PlayerEx* Character::GetPlayerEx() const { return this->playerex_; }

int Character::GetTalentElementalBurstLevel() const {
  return this->talent_elemental_burst_level_;
}

int Character::GetTalentElementalSkillLevel() const {
  return this->talent_elemental_skill_level_;
}

int Character::GetTalentNormalAttackLevel() const {
  return this->talent_normal_attack_level_;
}

void Character::IncreaseAscensionPhase() {
  if (this->GetLevel() ==
      Character::kAcensionPhaseMaxLevelList
          [this->ascension_phase_]) {  // if it is time to ascense
    this->ascension_phase_ = std::min(this->ascension_phase_ + 1, 6);
  }
}

void Character::IncreaseCharacterEXP(int value) {
  this->character_EXP_ += std::max(value, 0);
}

void Character::IncreaseConstellation() {
  this->constellation_ = std::min(this->constellation_ + 1, 6);
}

void Character::IncreaseHP(int value) {
  if (value > 0 && this->HP_ == 0) {
    return;
  }
  this->HP_ += value;
  this->HP_ = std::max(0, this->HP_);
  this->HP_ = std::min(this->GetStats().max_HP, this->HP_);
}

bool Character::IsDead() const { return (this->HP_ == 0); }

void Character::Revive() {
  if (this->HP_ == 0) {
    this->HP_ = 1;
  }
}

std::shared_ptr<Character> Character::Make(
    PlayerEx* playerex, const std::string& name, int ascension_phase,
    int character_EXP, int constellation, int HP,
    int talent_elemental_burst_level, int talent_elemental_skill_level,
    int talent_normal_attack_level) {
  if (name == "Kuki Shinobu") {
    return std::make_shared<KukiShinobu>(
        playerex, ascension_phase, character_EXP, constellation, HP,
        talent_elemental_burst_level, talent_elemental_skill_level,
        talent_normal_attack_level);
  }

  throw ExceptionNotACharacter();
}

const int Character::kAcensionPhaseMaxLevelList[7] = {20, 40, 50, 60,
                                                      70, 80, 90};

const int Character::kLevelMinCharacterEXPList[91] = {
    0,       0,       1000,    2325,    4025,    6175,    8800,    11950,
    15675,   20025,   25025,   30725,   37175,   44400,   52450,   61375,
    71200,   81950,   93675,   106400,  120175,  135050,  151850,  169850,
    189100,  209650,  231525,  254775,  279425,  305525,  333100,  362200,
    392850,  425100,  458975,  494525,  531775,  570750,  611500,  654075,
    698500,  744800,  795425,  848125,  902900,  959800,  1018875, 1080150,
    1143675, 1209475, 1277600, 1348075, 1424575, 1503625, 1585275, 1669550,
    1756500, 1846150, 1938550, 2033725, 2131725, 2232600, 2341550, 2453600,
    2568775, 2687100, 2808625, 2933400, 3061475, 3192875, 3327650, 3465825,
    3614525, 3766900, 3922975, 4082800, 4246400, 4413825, 4585125, 4760350,
    4939525, 5122700, 5338925, 5581950, 5855050, 6161850, 6506450, 6893400,
    7327825, 7815450, 8362650};

Character::Character(PlayerEx* playerex, int ascension_phase, int character_EXP,
                     int constellation, int HP,
                     int talent_elemental_burst_level,
                     int talent_elemental_skill_level,
                     int talent_normal_attack_level)
    : ascension_phase_(ascension_phase),
      character_EXP_(character_EXP),
      constellation_(constellation),
      HP_(HP),
      playerex_(playerex),
      talent_elemental_burst_level_(talent_elemental_burst_level),
      talent_elemental_skill_level_(talent_elemental_skill_level),
      talent_normal_attack_level_(talent_normal_attack_level) {
  // Validate the arguments
  if (ascension_phase < 0 || ascension_phase > 6) {
    throw ExceptionInvalidCharacterData();
  }
  if (character_EXP < 0) {
    throw ExceptionInvalidCharacterData();
  }
  if (constellation < 0 || constellation > 6) {
    throw ExceptionInvalidCharacterData();
  }
  if (HP < 0) {
    throw ExceptionInvalidCharacterData();
  }
  if (talent_elemental_burst_level < 0 || talent_elemental_burst_level > 10) {
    throw ExceptionInvalidCharacterData();
  }
  if (talent_elemental_skill_level < 0 || talent_elemental_skill_level > 10) {
    throw ExceptionInvalidCharacterData();
  }
  if (talent_normal_attack_level < 0 || talent_normal_attack_level > 10) {
    throw ExceptionInvalidCharacterData();
  }
}

}  // namespace genshicraft