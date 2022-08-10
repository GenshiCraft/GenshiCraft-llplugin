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
 * @file Plugin.cc
 * @author Futrime (futrime@outlook.com)
 * @brief The entrypoint of the plugin
 * @version 1.0.0
 * @date 2022-07-22
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "Plugin.h"

#include <EventAPI.h>
#include <LoggerAPI.h>
#include <ScheduleAPI.h>
#include <ServerAPI.h>

#include <MC/Actor.hpp>
#include <MC/ActorDamageSource.hpp>
#include <MC/Container.hpp>
#include <MC/ItemStack.hpp>
#include <MC/Player.hpp>

#include "Command.h"
#include "PlayerEx.h"
#include "Version.h"
#include "Weapon.h"
#include "exceptions.h"

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

void Init() {
  CheckProtocolVersion();

  Command::Init();
  PlayerEx::Init();
  Weapon::Init();

  Event::MobHurtEvent::subscribe_ref(OnMobHurt);
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
  if (event.mDamageSource->isEntitySource() &&
      event.mDamageSource->getEntity()->getTypeName() == "minecraft:player") {
    auto player = static_cast<Player*>(event.mDamageSource->getEntity());
    player->sendTitlePacket(std::to_string(static_cast<int>(event.mDamage)),
                            TitleType::SetActionBar, 0, 1, 0);
  }

  if (event.mMob->getTypeName() == "minecraft:player") {
    auto victim = static_cast<Player*>(event.mMob);
    auto victim_playerex = PlayerEx::Get(victim->getXuid());
    // The character takes damage
    victim_playerex->GetCharacter()->IncreaseHP(
        static_cast<int>(-event.mDamage * 10));
    event.mDamage = 0;
  }

  return true;
}

bool OnPlayerRespawn(Event::PlayerRespawnEvent& event) {
  PlayerEx::OnPlayerRespawn(event.mPlayer);

  return true;
}

bool OnPlayerInventoryChange(Event::PlayerInventoryChangeEvent& event) {
  auto playerex = PlayerEx::Get(event.mPlayer->getXuid());
  if (playerex == nullptr) {  // if the player is not loaded
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
  if (!playerex->IsOpeningContainer()) {  // if the player is opening their
                                          // inventory
    // something to do
  }
  playerex->SetIsOpeningContainer(false);
  return true;
}

void OnTick() { PlayerEx::OnTick(); }

}  // namespace genshicraft