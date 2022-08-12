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
 * @file plugin.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of global interfaces and objects
 * @version 1.0.0
 * @date 2022-07-22
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "plugin.h"

#include <EventAPI.h>
#include <LoggerAPI.h>
#include <ScheduleAPI.h>
#include <ServerAPI.h>

#include <MC/Actor.hpp>
#include <MC/ActorDamageSource.hpp>
#include <MC/Container.hpp>
#include <MC/ItemStack.hpp>
#include <MC/Player.hpp>
#include <chrono>

#include "command.h"
#include "damage.h"
#include "exceptions.h"
#include "playerex.h"
#include "version.h"
#include "weapon.h"

namespace genshicraft {

Logger logger(PLUGIN_NAME);

void CheckProtocolVersion() {
#ifdef TARGET_BDS_PROTOCOL_VERSION
  auto currentProtocol = LL::getServerProtocolVersion();
  if (TARGET_BDS_PROTOCOL_VERSION != currentProtocol) {
    logger.warn(
        "Protocol version not match, target version: {}, current version: {}.",
        TARGET_BDS_PROTOCOL_VERSION, currentProtocol);
    logger.warn(
        "This will most likely crash the server, please use the Plugin that "
        "matches the BDS version!");
  }
#endif  // TARGET_BDS_PROTOCOL_VERSION
}

double GetNowClock() {
  return static_cast<double>(
             std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::steady_clock::now().time_since_epoch())
                 .count()) /
         1000;
}

void Init() {
  CheckProtocolVersion();

  Command::Init();
  PlayerEx::Init();
  Weapon::Init();

  Event::MobHurtEvent::subscribe_ref(OnMobHurt);
  Event::PlayerDropItemEvent::subscribe_ref(OnPlayerDropItem);
  Event::PlayerExperienceAddEvent::subscribe_ref(OnPlayerExperienceAdd);
  Event::PlayerInventoryChangeEvent::subscribe_ref(OnPlayerInventoryChange);
  Event::PlayerJoinEvent::subscribe_ref(OnPlayerJoin);
  Event::PlayerLeftEvent::subscribe_ref(OnPlayerLeft);
  Event::PlayerOpenContainerEvent::subscribe_ref(OnPlayerOpenContainer);
  Event::PlayerOpenContainerScreenEvent::subscribe_ref(
      OnPlayerOpenContainerScreen);
  Event::PlayerRespawnEvent::subscribe_ref(OnPlayerRespawn);

  Schedule::repeat(OnTick, 1);
}

bool OnMobHurt(Event::MobHurtEvent& event) {
  double damage = event.mDamage * 10;

  // Damage source
  if (event.mDamageSource->isEntitySource() &&
      event.mDamageSource->getEntity()->getTypeName() ==
          "minecraft:player") {  // if the performer is a player
    auto player = static_cast<Player*>(event.mDamageSource->getEntity());
    auto playerex = PlayerEx::Get(player->getXuid());

    if (playerex->GetCharacter()->HasWeapon()) {
      damage = playerex->GetCharacter()->GetDamageNormalAttack().Get();
    }

    event.mDamage = static_cast<float>(damage / 10);

    if (event.mDamage > 0.0001) {
      player->sendTitlePacket(std::to_string(static_cast<int>(damage)),
                              TitleType::SetActionBar, 0, 1, 0);
    }
  }

  // Damage victim
  if (event.mMob->getTypeName() == "minecraft:player") {
    auto playerex = PlayerEx::Get(static_cast<Player*>(event.mMob)->getXuid());

    // Use GenshiCraft damage instead of Minecraft damage
    playerex->GetCharacter()->IncreaseHP(static_cast<int>(-damage));
    event.mDamage = 0;
  }

  // Prevent zero-damage hurt
  if (damage < 0.0001) {
    return false;
  }

  return true;
}

bool OnPlayerDropItem(Event::PlayerDropItemEvent& event) {
  // Prevent keeping inventory
  if (event.mPlayer->getHealth() == 0) {
    return true;
  }

  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  if (!playerex->GetPlayer()
           ->isSneaking()) {  // if the player is pressing Q without Shift
    if (playerex->GetCharacter()->HasWeapon()) {
      // Perform elemental burst

      return false;
    }
  } else {  // if the player is pressing Shift + Q
    playerex->GetMenu().OpenMain();

    return false;
  }

  return true;
}

bool OnPlayerExperienceAdd(Event::PlayerExperienceAddEvent& event) {
  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  playerex->GetCharacter()->IncreaseEnergy(event.mExp);

  return true;
}

bool OnPlayerRespawn(Event::PlayerRespawnEvent& event) {
  PlayerEx::OnPlayerRespawn(event.mPlayer);

  return true;
}

bool OnPlayerInventoryChange(Event::PlayerInventoryChangeEvent& event) {
  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  if (!playerex) {  // if the player is not loaded
    return true;
  }

  if (Weapon::CheckIsWeapon(event.mNewItemStack)) {
    Weapon::Make(event.mNewItemStack, playerex.get());
  }

  return true;
}

bool OnPlayerJoin(Event::PlayerJoinEvent& event) {
  PlayerEx::LoadPlayer(event.mPlayer);

  return true;
}

bool OnPlayerLeft(Event::PlayerLeftEvent& event) {
  PlayerEx::UnloadPlayer(event.mPlayer);

  return true;
}

bool OnPlayerOpenContainer(Event::PlayerOpenContainerEvent& event) {
  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  playerex->SetIsOpeningContainer(true);

  return true;
}

bool OnPlayerOpenContainerScreen(Event::PlayerOpenContainerScreenEvent& event) {
  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  if (!playerex->IsOpeningContainer() &&
      !playerex->GetPlayer()->isSneaking()) {  // if the player is pressing E
                                               // without pressing Shift
    if (playerex->GetCharacter()->HasWeapon()) {
      // Perform elemental skill

      return false;
    }
  }

  playerex->SetIsOpeningContainer(false);
  return true;
}

void OnTick() { PlayerEx::OnTick(); }

}  // namespace genshicraft