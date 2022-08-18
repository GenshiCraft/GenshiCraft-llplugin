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
 * @file menu.cc
 * @author Futrime (futrime@outlook.com)
 * @brief Definition of the Menu class
 * @version 1.0.0
 * @date 2022-07-22
 *
 * @copyright Copyright (c) 2022 Futrime
 *
 */

#include "menu.h"

#include <FormUI.h>
#include <ScheduleAPI.h>

#include <MC/ItemStack.hpp>
#include <MC/Player.hpp>
#include <MC/SimpleContainer.hpp>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "artifact.h"
#include "character.h"
#include "playerex.h"
#include "plugin.h"
#include "weapon.h"

namespace genshicraft {

Menu::Menu(PlayerEx* playerex) : playerex_(playerex) {
  // Empty
}

void Menu::OpenCharacter() {
  auto character = this->playerex_->GetCharacter();

  std::string content;

  content += " §f";
  for (int i = 0; i < 6; ++i) {
    if (i == character->GetAscensionPhase()) {
      content += "§7";
    }
    content += "✦";
  }
  content += "\n";

  content +=
      "§fLevel " + std::to_string(character->GetLevel()) + " §7/ " +
      std::to_string(
          Character::kAcensionPhaseMaxLevelList[character
                                                    ->GetAscensionPhase()]) +
      "\n";

  content += "§f";
  for (const auto& line : character->GetStatsDescription(false)) {
    content += "§f" + line + "\n";
  }

  Form::SimpleForm form("Character / " + character->GetName(), content);

  form = form.addButton(
      "Details", "", [this](Player* player) { this->OpenCharacterDetails(); });

  if (Character::kAcensionPhaseMaxLevelList[character->GetAscensionPhase()] ==
      character->GetLevel()) {
    if (character->GetLevel() != 90) {
      form = form.addButton("Ascend", "", [this](Player* player) {
        this->OpenCharacterAscend();
      });
    }
  } else {
    form = form.addButton("Level Up", "", [this](Player* player) {
      this->OpenCharacterLevelUp();
    });
  }

  // Show the weapon menu only if the player is holding a GenshiCraft weapon
  if (character->HasWeapon()) {
    form = form.addButton(
        "Weapon", "", [this](Player* player) { this->OpenCharacterWeapon(); });
  }

  // Show the artifacts menu only if the player is equipped with at least one
  // GenshiCraft artifact
  if (Artifact::CheckIsArtifact(
          this->playerex_->GetPlayer()->getArmorContainer().getSlot(0)) ||
      Artifact::CheckIsArtifact(
          this->playerex_->GetPlayer()->getArmorContainer().getSlot(1)) ||
      Artifact::CheckIsArtifact(
          this->playerex_->GetPlayer()->getArmorContainer().getSlot(2)) ||
      Artifact::CheckIsArtifact(
          this->playerex_->GetPlayer()->getArmorContainer().getSlot(3)) ||
      Artifact::CheckIsArtifact(const_cast<ItemStack*>(
          &(this->playerex_->GetPlayer()->getOffhandSlot())))) {
    form = form.addButton("Artifacts", "", [this](Player* player) {
      this->OpenCharacterArtifacts();
    });
  }

  // form = form.addButton("Constellation");

  // form = form.addButton("Talents");

  form.sendTo(this->playerex_->GetPlayer(), [this](Player* player, int option) {
    if (option == -1) {
      this->OpenMain();
      return;
    }
  });
}

void Menu::OpenCharacterArtifacts() {
  Form::SimpleForm form("Artifacts", "");

  auto player = this->playerex_->GetPlayer();

  bool has_artifact = false;

  if (Artifact::CheckIsArtifact(player->getArmorContainer().getSlot(0))) {
    form = form.addButton(
        "Flower of Life", "textures/menu/artifacts/flower_of_life.png",
        [this](Player* player) {
          this->OpenCharacterArtifacts(Artifact::Type::kFlowerOfLife);
        });
    has_artifact = true;
  }

  if (Artifact::CheckIsArtifact(player->getArmorContainer().getSlot(1))) {
    form = form.addButton(
        "Plume of Death", "textures/menu/artifacts/plume_of_death.png",
        [this](Player* player) {
          this->OpenCharacterArtifacts(Artifact::Type::kPlumeOfDeath);
        });
    has_artifact = true;
  }

  if (Artifact::CheckIsArtifact(player->getArmorContainer().getSlot(2))) {
    form = form.addButton(
        "Sands of Eon", "textures/menu/artifacts/sands_of_eon.png",
        [this](Player* player) {
          this->OpenCharacterArtifacts(Artifact::Type::kSandsOfEon);
        });
    has_artifact = true;
  }

  if (Artifact::CheckIsArtifact(player->getArmorContainer().getSlot(3))) {
    form = form.addButton(
        "Goblet of Eonothem", "textures/menu/artifacts/goblet_of_eonothem.png",
        [this](Player* player) {
          this->OpenCharacterArtifacts(Artifact::Type::kGobletOfEonothem);
        });
    has_artifact = true;
  }

  if (Artifact::CheckIsArtifact(
          const_cast<ItemStack*>(&(player->getOffhandSlot())))) {
    form = form.addButton(
        "Circlet of Logos", "textures/menu/artifacts/circlet_of_logos.png",
        [this](Player* player) {
          this->OpenCharacterArtifacts(Artifact::Type::kCircletOfLogos);
        });
    has_artifact = true;
  }

  if (!has_artifact) {
    this->OpenCharacter();
    return;
  }

  form.sendTo(this->playerex_->GetPlayer(), [this](Player* player, int option) {
    if (option == -1) {
      this->OpenCharacter();
      return;
    }
  });
}

void Menu::OpenCharacterArtifacts(Artifact::Type type) {
  if ((type == Artifact::Type::kFlowerOfLife &&
       !Artifact::CheckIsArtifact(
           this->playerex_->GetPlayer()->getArmorContainer().getSlot(0))) ||
      (type == Artifact::Type::kPlumeOfDeath &&
       !Artifact::CheckIsArtifact(
           this->playerex_->GetPlayer()->getArmorContainer().getSlot(1))) ||
      (type == Artifact::Type::kSandsOfEon &&
       !Artifact::CheckIsArtifact(
           this->playerex_->GetPlayer()->getArmorContainer().getSlot(2))) ||
      (type == Artifact::Type::kGobletOfEonothem &&
       !Artifact::CheckIsArtifact(
           this->playerex_->GetPlayer()->getArmorContainer().getSlot(3))) ||
      (type == Artifact::Type::kCircletOfLogos &&
       !Artifact::CheckIsArtifact(const_cast<ItemStack*>(
           &(this->playerex_->GetPlayer()->getOffhandSlot()))))) {
    this->OpenCharacterArtifacts();
    return;
  }
}

void Menu::OpenCharacterAscend() {
  auto character = this->playerex_->GetCharacter();

  // Check if it is time to ascend
  if ((Character::kAcensionPhaseMaxLevelList[character->GetAscensionPhase()] !=
       character->GetLevel()) ||
      character->GetLevel() == 90) {
    this->OpenCharacter();
    return;
  }

  bool can_ascend = true;
  for (const auto& item : character->GetAscensionMaterials()) {
    if (item.first == "genshicraft:mora_1") {
      if (this->playerex_->GetMoraCount() < item.second) {
        can_ascend = false;
      }
    } else {
      if (this->playerex_->GetItemCount(item.first) < item.second) {
        can_ascend = false;
      }
    }
  }

  if (!can_ascend) {
    std::string content;

    content += "§cNot enough ascension materials!\n\n";
    content += "§fAscension materials:\n";

    for (const auto& item : character->GetAscensionMaterials()) {
      std::string item_name;
      if (item.first == "genshicraft:mora_1") {
        item_name = "§bMora";
      } else {
        item_name = ItemStack::create(item.first)->getName();
      }
      content += "§f" + item_name + " §fx" + std::to_string(item.second) + "\n";
    }

    Form::SimpleForm form("Ascend / " + character->GetName(), content);

    form.sendTo(this->playerex_->GetPlayer(),
                [this](Player* player, int option) {
                  if (option == -1) {
                    this->OpenCharacter();
                    return;
                  }
                });
  } else {
    Form::SimpleForm form("Ascend / " + character->GetName(),
                          "Are you sure to ascend?");

    form.addButton("Continue");

    form.sendTo(this->playerex_->GetPlayer(),
                [this](Player* player, int option) {
                  if (option == -1) {
                    this->OpenCharacter();
                    return;
                  }

                  auto character = this->playerex_->GetCharacter();

                  for (const auto& item : character->GetAscensionMaterials()) {
                    if (item.first == "genshicraft:mora_1") {
                      this->playerex_->ConsumeMora(item.second);
                    } else {
                      this->playerex_->ConsumeItem(item.first, item.second);
                    }
                  }

                  character->IncreaseAscensionPhase();

                  this->OpenCharacter();
                  return;
                });
  }
}

void Menu::OpenCharacterDetails() {
  auto character = this->playerex_->GetCharacter();

  std::string content;

  content += "§f";
  auto stats_description = character->GetStatsDescription(true);
  for (int i = 0; i < stats_description.size(); ++i) {
    if (i == 0) {
      content += "§7Base Stats\n";
    } else if (i == 5) {
      content += "\n§7Advanced Stats\n";
    } else if (i == 12) {
      content += "\n§7Elemental Type\n";
    }
    content += "§f" + stats_description[i] + "\n";
  }

  Form::SimpleForm form("Details / " + character->GetName(), content);

  form.sendTo(this->playerex_->GetPlayer(), [this](Player* player, int option) {
    if (option == -1) {
      this->OpenCharacter();
      return;
    }
  });
}

void Menu::OpenCharacterLevelUp() {
  auto character = this->playerex_->GetCharacter();

  // Check if it is time to level up
  if (Character::kAcensionPhaseMaxLevelList[character->GetAscensionPhase()] ==
      character->GetLevel()) {
    this->OpenCharacter();
    return;
  }

  auto max_character_exp =
      this->playerex_->GetItemCount("genshicraft:wanderer_s_advice") * 1000 +
      this->playerex_->GetItemCount("genshicraft:adventurer_s_experience") *
          5000 +
      this->playerex_->GetItemCount("genshicraft:hero_s_wit") * 20000;

  auto max_up_level = character->GetLevelByCharacterEXP(
      max_character_exp + character->GetCharacterEXP());

  Form::CustomForm form("Level Up / " + character->GetName());

  form = form.addLabel("text_level",
                       "Lv." + std::to_string(character->GetLevel()));

  form.addSlider("level", "Level(s) to up", 0,
                 max_up_level - character->GetLevel());

  form.sendTo(
      this->playerex_->GetPlayer(),
      [this](Player* player,
             std::map<string, std::shared_ptr<Form::CustomFormElement>> data) {
        // Go back to main menu if the player cancels the form
        if (data.empty()) {
          this->OpenCharacter();
          return;
        }

        auto character = this->playerex_->GetCharacter();

        auto enhanced_level =
            data.at("level")->getInt() + character->GetLevel();

        while (character->GetLevel() < enhanced_level &&
               this->playerex_->GetItemCount("genshicraft:hero_s_wit") > 0) {
          this->playerex_->ConsumeItem("genshicraft:hero_s_wit", 1);
          character->IncreaseCharacterEXP(20000);
        }

        while (character->GetLevel() < enhanced_level &&
               this->playerex_->GetItemCount(
                   "genshicraft:adventurer_s_experience") > 0) {
          this->playerex_->ConsumeItem("genshicraft:adventurer_s_experience",
                                       1);
          character->IncreaseCharacterEXP(5000);
        }

        while (character->GetLevel() < enhanced_level &&
               this->playerex_->GetItemCount("genshicraft:wanderer_s_advice") >
                   0) {
          this->playerex_->ConsumeItem("genshicraft:wanderer_s_advice", 1);
          character->IncreaseCharacterEXP(1000);
        }

        Schedule::nextTick([this]() { this->OpenCharacterLevelUp(); });
      });
}

void Menu::OpenCharacterWeapon() {
  static const std::string kRefinementSymbolList[6] = {" ", "①", "②",
                                                       "③", "④", "§6⑤"};

  // Check if the player is holding a GenshiCraft weapon
  if (!this->playerex_->GetWeapon()) {
    this->OpenCharacter();
    return;
  }

  auto weapon = this->playerex_->GetWeapon();

  std::string content;

  content += "§7";
  if (weapon->GetType() == Weapon::Type::kSword) {
    content += "Sword";
  } else if (weapon->GetType() == Weapon::Type::kClaymore) {
    content += "Claymore";
  } else if (weapon->GetType() == Weapon::Type::kPolearm) {
    content += "Polearm";
  } else if (weapon->GetType() == Weapon::Type::kCatalyst) {
    content += "Catalyst";
  } else if (weapon->GetType() == Weapon::Type::kBow) {
    content += "Bow";
  }
  content += "\n";

  for (const auto& line : weapon->GetBaseStatsDescription()) {
    content += "§f" + line + "\n";
  }

  content += "§6";
  for (int i = 0; i < weapon->GetRarity(); ++i) {
    content += "★";
  }
  content += "\n";

  content +=
      "§fLv." + std::to_string(weapon->GetLevel()) + " §7/ " +
      std::to_string(
          Weapon::kAcensionPhaseMaxLevelList[weapon->GetAscensionPhase()]);
  content += " §f";
  for (int i = 0; i < ((weapon->GetRarity() <= 2) ? 4 : 6); ++i) {
    if (i == weapon->GetAscensionPhase()) {
      content += "§7";
    }
    content += "✦";
  }
  content += "\n";

  if (weapon->GetRarity() >= 3) {
    content +=
        "§eRefinement Rank " + std::to_string(weapon->GetRefinement()) + "\n";
  }

  Form::SimpleForm form("Weapon / " + this->playerex_->GetWeapon()->GetName(),
                        content);

  if (Weapon::kAcensionPhaseMaxLevelList[weapon->GetAscensionPhase()] ==
      weapon->GetLevel()) {
    if ((weapon->GetRarity() >= 3 && weapon->GetLevel() != 90) ||
        (weapon->GetRarity() <= 2 && weapon->GetLevel() != 70)) {
      form = form.addButton("Ascend", "", [this](Player* player) {
        this->OpenCharacterWeaponAscend();
      });
    }
  } else {
    form = form.addButton("Enhance", "", [this](Player* player) {
      this->OpenCharacterWeaponEnhance();
    });
  }

  if (weapon->GetRarity() >= 3 && weapon->GetRefinement() < 5) {
    // form = form.addButton("Refine", "", [this](Player* player) {
    //   this->OpenCharacterWeaponRefine();
    // });
  }

  form.sendTo(this->playerex_->GetPlayer(), [this](Player* player, int option) {
    if (option == -1) {
      this->OpenCharacter();
      return;
    }
  });
}

void Menu::OpenCharacterWeaponAscend() {
  // Check if the player is holding a GenshiCraft weapon
  if (!this->playerex_->GetWeapon()) {
    this->OpenCharacter();
    return;
  }

  auto weapon = this->playerex_->GetWeapon();

  // Check if it is time to ascend
  if ((Weapon::kAcensionPhaseMaxLevelList[weapon->GetAscensionPhase()] !=
       weapon->GetLevel()) ||
      ((weapon->GetRarity() >= 3 && weapon->GetLevel() == 90) ||
       (weapon->GetRarity() <= 2 && weapon->GetLevel() == 70))) {
    this->OpenCharacterWeapon();
    return;
  }

  bool can_ascend = true;
  for (const auto& item : weapon->GetAscensionMaterials()) {
    if (item.first == "genshicraft:mora_1") {
      if (this->playerex_->GetMoraCount() < item.second) {
        can_ascend = false;
      }
    } else {
      if (this->playerex_->GetItemCount(item.first) < item.second) {
        can_ascend = false;
      }
    }
  }

  if (!can_ascend) {
    std::string content;

    content += "§cNot enough ascension materials!\n\n";
    content += "§fAscension materials:\n";

    for (const auto& item : weapon->GetAscensionMaterials()) {
      std::string item_name;
      if (item.first == "genshicraft:mora_1") {
        item_name = "§bMora";
      } else {
        item_name = ItemStack::create(item.first)->getName();
      }
      content += "§f" + item_name + " §fx" + std::to_string(item.second) + "\n";
    }

    Form::SimpleForm form("Ascend / " + weapon->GetName(),
                          content);  // the first 2 characters of the weapon
                                     // name are the formatting code

    form.sendTo(this->playerex_->GetPlayer(),
                [this](Player* player, int option) {
                  if (option == -1) {
                    this->OpenCharacterWeapon();
                    return;
                  }
                });
  } else {
    Form::SimpleForm form(
        "Ascend / " + weapon->GetName(),
        "Are you sure to ascend?");  // the first 2 characters of the weapon
                                     // name are the formatting code

    form.addButton("Continue");

    form.sendTo(this->playerex_->GetPlayer(),
                [this](Player* player, int option) {
                  if (option == -1) {
                    this->OpenCharacterWeapon();
                    return;
                  }

                  auto weapon = this->playerex_->GetWeapon();

                  for (const auto& item : weapon->GetAscensionMaterials()) {
                    if (item.first == "genshicraft:mora_1") {
                      this->playerex_->ConsumeMora(item.second);
                    } else {
                      this->playerex_->ConsumeItem(item.first, item.second);
                    }
                  }

                  weapon->IncreaseAscensionPhase();

                  Schedule::nextTick([this]() { this->OpenCharacterWeapon(); });
                  return;
                });
  }
}

void Menu::OpenCharacterWeaponEnhance() {
  // Check if the player is holding a GenshiCraft weapon
  if (!this->playerex_->GetWeapon()) {
    this->OpenCharacter();
    return;
  }

  auto weapon = this->playerex_->GetWeapon();

  // Check if it is time to enhance
  if (Weapon::kAcensionPhaseMaxLevelList[weapon->GetAscensionPhase()] ==
      weapon->GetLevel()) {
    this->OpenCharacterWeapon();
    return;
  }

  auto max_weapon_exp =
      this->playerex_->GetItemCount("genshicraft:enhancement_ore") * 400 +
      this->playerex_->GetItemCount("genshicraft:fine_enhancement_ore") * 2000 +
      this->playerex_->GetItemCount("genshicraft:mystic_enhancement_ore") *
          10000;

  auto max_enhanced_level =
      weapon->GetLevelByWeaponEXP(max_weapon_exp + weapon->GetWeaponEXP());

  Form::CustomForm form("Enhance / " + weapon->GetName());

  form =
      form.addLabel("text_level", "Lv." + std::to_string(weapon->GetLevel()));

  auto description = weapon->GetBaseStatsDescription();
  for (int i = 0; i < description.size(); ++i) {
    form = form.addLabel("text_weapon_stats_" + std::to_string(i),
                         description.at(i));
  }

  form.addSlider("level", "Level(s) to enhance", 0,
                 max_enhanced_level - weapon->GetLevel());

  form.sendTo(
      this->playerex_->GetPlayer(),
      [this](Player* player,
             std::map<string, std::shared_ptr<Form::CustomFormElement>> data) {
        // Go back to main menu if the player cancels the form
        if (data.empty()) {
          this->OpenCharacterWeapon();
          return;
        }

        auto weapon = this->playerex_->GetWeapon();

        auto enhanced_level = data.at("level")->getInt() + weapon->GetLevel();

        while (weapon->GetLevel() < enhanced_level &&
               this->playerex_->GetItemCount(
                   "genshicraft:mystic_enhancement_ore") > 0) {
          this->playerex_->ConsumeItem("genshicraft:mystic_enhancement_ore", 1);
          weapon->IncreaseWeaponEXP(10000);
        }

        while (weapon->GetLevel() < enhanced_level &&
               this->playerex_->GetItemCount(
                   "genshicraft:fine_enhancement_ore") > 0) {
          this->playerex_->ConsumeItem("genshicraft:fine_enhancement_ore", 1);
          weapon->IncreaseWeaponEXP(2000);
        }

        while (weapon->GetLevel() < enhanced_level &&
               this->playerex_->GetItemCount("genshicraft:enhancement_ore") >
                   0) {
          this->playerex_->ConsumeItem("genshicraft:enhancement_ore", 1);
          weapon->IncreaseWeaponEXP(400);
        }

        Schedule::nextTick([this]() { this->OpenCharacterWeaponEnhance(); });
      });
}

void Menu::OpenCharacterWeaponRefine() {
  // Check if the player is holding a GenshiCraft weapon
  if (!this->playerex_->GetWeapon()) {
    this->OpenCharacter();
    return;
  }
}

void Menu::OpenMain() {
  Form::SimpleForm form("GenshiCraft Menu", "");

  // form = form.addButton("Shop", "textures/menu/shop.bmp");

  form = form.addButton("Party Setup", "textures/menu/party_setup.bmp",
                        [this](Player* player) { this->OpenPartySetup(); });

  form = form.addButton("Character", "textures/menu/character.bmp",
                        [this](Player* player) { this->OpenCharacter(); });

  // form = form.addButton("Wish", "textures/menu/wish.bmp");

  form.sendTo(this->playerex_->GetPlayer());
}

void Menu::OpenPartySetup() {
  Form::SimpleForm form(
      "Party Setup",
      "Current character: " + this->playerex_->GetCharacter()->GetName());

  for (auto&& character : this->playerex_->GetAllCharacters()) {
    form =
        form.addButton(character->GetName(), "textures/menu/characters/" +
                                                 character->GetName() + ".png");
  }

  form.sendTo(this->playerex_->GetPlayer(), [this](Player* player, int option) {
    if (option == -1) {
      this->OpenMain();
      return;
    }

    this->playerex_->SetCharacter(option);

    this->OpenPartySetup();
  });
}

}  // namespace genshicraft
