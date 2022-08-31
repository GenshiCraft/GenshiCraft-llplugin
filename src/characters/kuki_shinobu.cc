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
 * @file kuki_shinobu.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the KukiShinobu class
 * @version 1.0.0
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "kuki_shinobu.h"

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "character.h"
#include "damage.h"
#include "exceptions.h"
#include "playerex.h"
#include "plugin.h"
#include "stats.h"
#include "weapon.h"
#include "world.h"

namespace genshicraft {

KukiShinobu::KukiShinobu(PlayerEx* playerex, int ascension_phase,
                         int character_EXP, int constellation, int energy,
                         int HP, int talent_elemental_burst_level,
                         int talent_elemental_skill_level,
                         int talent_normal_attack_level)
    : Character(playerex, ascension_phase, character_EXP, constellation, energy,
                HP, talent_elemental_burst_level, talent_elemental_skill_level,
                talent_normal_attack_level) {
  if (HP > this->GetStats().GetMaxHP()) {
    throw ExceptionInvalidCharacterData();
  }
}

std::map<std::string, int> KukiShinobu::GetAscensionMaterials() const {
  return KukiShinobu::kAscensionMaterialsList[this->GetAscensionPhase()];
}

Damage KukiShinobu::GetAttackDamage(const world::AttackType& attack_type) {
  static auto last_hit_clock = GetNowClock() - 999999.;
  static int hit_count = 0;
  static int charged_attack_count = 0;

  Damage damage;

  switch (attack_type) {
    case world::AttackType::kHit:

      // Reset hit count
      if (GetNowClock() - last_hit_clock > 5.) {
        hit_count = 0;
      }

      // The attack cooldown
      if (GetNowClock() - last_hit_clock < 0.55) {
        return Damage();
      }

      damage.SetAttackerAmplifier(
          this->kTalentNormalAttackHitDMG.at(hit_count).at(
              this->GetTalentLevel(world::TalentType::kNormalAttack)));

      ++hit_count;
      if (hit_count > 3) {
        hit_count = 0;
      }

      charged_attack_count = 0;

      break;

    case world::AttackType::kChargedAttack:

      if (this->GetPlayerEx()->GetStamina() <
          20) {  // the charged attack can only be
                 // performed when the stamina is enough
        break;
      }

      // Reset hit count if not attacking for more than 5s
      if (GetNowClock() - last_hit_clock > 5.) {
        charged_attack_count = 0;
      }

      // The attack cooldown
      if (GetNowClock() - last_hit_clock < 1.1 && charged_attack_count == 0) {
        return Damage();
      }

      damage.SetAttackerAmplifier(
          this->kTalentNormalAttackChargedAttackDMG.at(charged_attack_count)
              .at(this->GetTalentLevel(world::TalentType::kNormalAttack)));

      ++charged_attack_count;
      if (charged_attack_count > 1) {
        charged_attack_count = 0;
      }

      hit_count = 0;

      break;

    case world::AttackType::kPlungeHigh:
      damage.SetAttackerAmplifier(this->kTalentNormalAttackPlungeHighDMG.at(
          this->GetTalentLevel(world::TalentType::kNormalAttack)));

      charged_attack_count = 0;
      hit_count = 0;

      break;

    case world::AttackType::kPlungeLow:

      damage.SetAttackerAmplifier(this->kTalentNormalAttackPlungeLowDMG.at(
          this->GetTalentLevel(world::TalentType::kNormalAttack)));

      charged_attack_count = 0;
      hit_count = 0;

      break;

    case world::AttackType::kPlungeRegular:

      damage.SetAttackerAmplifier(this->kTalentNormalAttackPlungeRegularDMG.at(
          this->GetTalentLevel(world::TalentType::kNormalAttack)));

      charged_attack_count = 0;
      hit_count = 0;

      break;

    default:
      throw ExceptionInvalidAttackType();
  }

  damage.SetAttackElementType(world::ElementType::kPhysical);
  damage.SetAttackerLevel(this->GetLevel());
  damage.SetAttackerStats(this->GetStats());
  damage.SetSourceType(Damage::SourceType::kMob);

  last_hit_clock = GetNowClock();

  return damage;
}

Stats KukiShinobu::GetBaseStats() const {
  Stats stats;
  stats.max_HP_base = KukiShinobu::kStatsMaxHPBase[this->GetAscensionPhase()] +
                      KukiShinobu::kStatsMaxHPDiff * this->GetLevel();
  stats.ATK_base = KukiShinobu::kStatsATKBase[this->GetAscensionPhase()] +
                   KukiShinobu::kStatsATKDiff * this->GetLevel();
  stats.DEF_base = KukiShinobu::kStatsDEFBase[this->GetAscensionPhase()] +
                   KukiShinobu::kStatsDEFDiff * this->GetLevel();

  stats.max_HP_percent =
      KukiShinobu::kStatsMaxHPPercent[this->GetAscensionPhase()];

  return stats;
}

double KukiShinobu::GetCDMax(const world::TalentType& talent) const {
  switch (talent) {
    case world::TalentType::kElementalBurst:
      return 15.;
      break;

    case world::TalentType::kElementalSkill:
      return 15.;
      break;

    default:
      return 0.;
      break;
  }
}

int KukiShinobu::GetEnergyMax() const { return 60; }

std::string KukiShinobu::GetName() const { return "Kuki Shinobu"; }

int KukiShinobu::GetRarity() const { return 4; }

std::map<std::string, int> KukiShinobu::GetTalentLevelUpMaterials(
    const world::TalentType& talent) const {
  auto talent_level = this->GetTalentLevel(talent);
  return this->kTalentLevelUpMaterialList.at(talent_level);
}

bool KukiShinobu::HasWeapon() const {
  if (this->GetPlayerEx()->GetWeapon() &&
      this->GetPlayerEx()->GetWeapon()->GetType() == Weapon::Type::kSword) {
    return true;
  } else {
    return false;
  }
}

const std::vector<std::map<std::string, int>>
    KukiShinobu::kAscensionMaterialsList = {{{"genshicraft:mora_1", 20000}},
                                            {{"genshicraft:mora_1", 40000}},
                                            {{"genshicraft:mora_1", 60000}},
                                            {{"genshicraft:mora_1", 80000}},
                                            {{"genshicraft:mora_1", 100000}},
                                            {{"genshicraft:mora_1", 120000}},
                                            {}};

const std::vector<int> KukiShinobu::kStatsATKBase = {17, 39,  58, 75,
                                                     90, 104, 118};

const int KukiShinobu::kStatsATKDiff = 1;

const std::vector<int> KukiShinobu::kStatsDEFBase = {58,  109, 146, 193,
                                                     228, 263, 299};

const int KukiShinobu::kStatsDEFDiff = 5;

const std::vector<int> KukiShinobu::kStatsMaxHPBase = {945,  1717, 2265, 2995,
                                                       3543, 4090, 4638};

const int KukiShinobu::kStatsMaxHPDiff = 85;

const std::vector<double> KukiShinobu::kStatsMaxHPPercent = {
    0, 0, 0.06, 0.12, 0.12, 0.18, 0.24};

const std::vector<std::vector<double>>
    KukiShinobu::kTalentNormalAttackChargedAttackDMG = {
        {0., 0.5563, 0.6016, 0.6469, 0.7116, 0.7569, 0.8086, 0.8798, 0.9509,
         1.0221, 1.0997, 1.1774},
        {0., 0.6677, 0.722, 0.7763, 0.854, 0.9083, 0.9704, 1.0558, 1.1412,
         1.2266, 1.3198, 1.4129}};

const std::vector<std::vector<double>> KukiShinobu::kTalentNormalAttackHitDMG =
    {
        {0, 0.4876, 0.5273, 0.5670, 0.6237, 0.6634, 0.7088, 0.7711, 0.8335,
         0.8959, 0.9639, 1.0319},
        {0, 0.4455, 0.4817, 0.5180, 0.5698, 0.6061, 0.6475, 0.7045, 0.7615,
         0.8184, 0.8806, 0.9428},
        {0, 0.5934, 0.6417, 0.6900, 0.7590, 0.8073, 0.8625, 0.9384, 1.0143,
         1.0902, 1.1730, 1.2558},
        {0, 0.7611, 0.8230, 0.8850, 0.9735, 1.0355, 1.1063, 1.2036, 1.3009,
         1.3983, 1.5045, 1.6107},
};

const std::vector<double> KukiShinobu::kTalentNormalAttackPlungeHighDMG = {
    0,      1.5968, 1.7267, 1.8567, 2.0424, 2.1723,
    2.3209, 2.5251, 2.7293, 2.9336, 3.1564, 3.3792};

const std::vector<double> KukiShinobu::kTalentNormalAttackPlungeLowDMG = {
    0,      1.2784, 1.3824, 1.4865, 1.6351, 1.7392,
    1.8581, 2.0216, 2.1851, 2.3486, 2.5270, 2.7054};

const std::vector<double> KukiShinobu::kTalentNormalAttackPlungeRegularDMG = {
    0,      0.6393, 0.6914, 0.7434, 0.8177, 0.8698,
    0.9292, 1.011,  1.0928, 1.1746, 1.2638, 1.353};

const std::vector<std::map<std::string, int>> KukiShinobu::kTalentLevelUpMaterialList = {
    {},
    {{"genshicraft:mora_1", 12500}},
    {{"genshicraft:mora_1", 17500}},
    {{"genshicraft:mora_1", 25000}},
    {{"genshicraft:mora_1", 30000}},
    {{"genshicraft:mora_1", 37500}},
    {{"genshicraft:mora_1", 120000}},
    {{"genshicraft:mora_1", 260000}},
    {{"genshicraft:mora_1", 450000}},
    {{"genshicraft:mora_1", 700000}}};

}  // namespace genshicraft
