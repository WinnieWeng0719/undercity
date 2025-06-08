#include "game_functions.h"
#include "Item.h"
#include "Weapon.h"
#include "Potion.h"
#include "Character.h"
#include <iostream>
#include <string>
#include <cstdlib> // For rand()
#include <ctime>   // For time() in main (though not in this file, it's a common dependency)
#include <algorithm> // For std::max

Character generateEnemy(int playerLevel) {
    int enemyType = rand() % 100;
    int level = rand() % 3 + 1;
    if (playerLevel >= 12 && enemyType < 20) {
        return Character("宇宙無敵超級深淵惡魔-龍龍", 500, rand() % 4 + 200, 20);
    }
    else if (playerLevel >= 9 && enemyType < 30) {
        return Character("你的作業", 250, rand() % 40 + 100, 10);
    }
    else if (playerLevel >= 6 && enemyType < 40) {
        return Character("林克", 100, rand() % 25 + 80, 15);
    }
    else if (playerLevel >= 3 && enemyType < 50) {
        return Character("芙莉蓮花", 50, rand() % 10 + 45, 5);
    }
    else if (enemyType < 60) {
        int health = (level == 1) ? 12 : (level == 2) ? 15 : 17;
        return Character("哥布林 (等級 " + std::to_string(level) + ")", health, rand() % 3 + 8, 1);
    }
    else if (enemyType < 70) {
        return Character("哥布林士兵", 30, rand() % 4 + 10, 3);
    }
    else if (enemyType < 80) {
        return Character("哥布林弓箭手", 25, rand() % 5 + 12, 2);
    }
    else {
        return Character("哥布林狂戰士", 35, rand() % 6 + 18, 4);
    }
}

void handleItemDrop(Character& player, const std::string& itemName, Item* droppedItem, const std::string& icon) {
    char choice;
    bool validInput = false;
    while (!validInput) {
        std::cout << icon << " 敵人掉落了" << itemName << "，是否拾取？（y/n）：";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            player.addItem(droppedItem);
            std::cout << "玩家拾取了 " << itemName << "！\n";
            validInput = true;
        }
        else if (choice == 'n' || choice == 'N') {
            std::cout << "玩家不拾取" << itemName << "。\n";
            delete droppedItem;
            validInput = true;
        }
        else {
            std::cout << "無效選擇，請重新輸入。\n";
        }
    }
}

void battle(Character& player, Character& enemy) {
    std::cout << "一隻 " << enemy.getName() << " 出現了！\n" << std::endl;

    while (player.isAlive() && enemy.isAlive()) {
        player.decreaseStrengthTurns();
        enemy.takePoisonDamage();

        player.displayStats();
        enemy.displayStats();

        if (!enemy.isAlive()) {
            std::cout << enemy.getName() << " 因中毒而死亡！\n" << std::endl;
            player.gainExperience(4 + player.getLevelValue());
            break;
        }

        if (enemy.getHealth() > player.getHealth()) {
            std::cout << "⚠️ 警告：血量過低!請小心應戰！\n";
        }

        std::cout << "\n";
        std::cout << "╔═════════════════╗" << std::endl;
        std::cout << "║    選擇攻擊     ║" << std::endl;
        std::cout << "╚═════════════════╝" << std::endl;
        std::cout << "1. 普通攻擊\n";
        std::cout << "2. 強力攻擊\n";
        std::cout << "3. 使用武器攻擊";
        if (player.getEquippedWeapon() != nullptr) {
            std::cout << " (" << player.getEquippedWeapon()->getName() << ")\n";
        }
        else {
            std::cout << " (未裝備武器)\n";
        }
        std::cout << "4. 使用物品\n";
        std::cout << "選擇動作: ";
        int action;
        std::cin >> action;
        std::cout << std::endl;

        int damage = 0;
        if (action == 1) {
            damage = player.getAttack();
            std::cout << player.getName() << " 選擇 [普通攻擊] ，造成了 " << damage << " 點傷害。\n";
            std::cout << enemy.getName() << " 的防禦力為他抵擋了 " << enemy.getDefense() << " 點傷害。\n";
            std::cout << std::endl << enemy.getName() << " 受到了 " << std::max(0, damage - enemy.getDefense()) << " 點傷害。\n" << std::endl;
            enemy.takeDamage(damage);
        }
        else if (action == 2) {
            damage = player.getAttack() * 2;
            std::cout << player.getName() << " 選擇 [強力攻擊] ，造成了 " << damage << " 點傷害。\n";
            std::cout << enemy.getName() << " 的防禦力抵擋了 " << enemy.getDefense() << " 點傷害。\n";
            std::cout << std::endl << enemy.getName() << " 受到了 " << std::max(0, damage - enemy.getDefense()) << " 點傷害。\n" << std::endl;
            enemy.takeDamage(damage);
        }
        else if (action == 3) {
            if (player.getEquippedWeapon() != nullptr) {
                player.getEquippedWeapon()->use();
                damage = player.getAttack() * 2 + (0.5 * player.getEquippedWeapon()->getValue());
                std::cout << player.getName() << " 使用 " << player.getEquippedWeapon()->getName() << " 攻擊，造成了 " << damage << " 點傷害。\n";
                std::cout << std::endl << enemy.getName() << " 的防禦力抵擋了 " << enemy.getDefense() << " 點傷害。\n";
                std::cout << std::endl << enemy.getName() << " 受到了 " << std::max(0, damage - enemy.getDefense()) << " 點傷害。\n" << std::endl;
                enemy.takeDamage(damage);
                if (player.getEquippedWeapon()->isBroken()) {
                    std::cout << player.getEquippedWeapon()->getName() << " 損壞了！\n";
                    player.unequipWeapon(true);
                }
            }
            else {
                std::cout << "玩家無裝備武器，無法使用武器攻擊。\n" << std::endl;
                continue;
            }
        }
        else if (action == 4) {
            if (player.getInventory().empty()) {
                std::cout << "物品清單是空的，沒有物品可以使用。\n";
                continue;
            }
            player.displayItemList();
            std::cout << "選擇一個物品來使用: ";
            int itemChoice;
            std::cin >> itemChoice;
            if (itemChoice > 0 && itemChoice <= player.getInventory().size()) {
                Item* selectedItem = player.getInventory()[itemChoice - 1];
                if (dynamic_cast<PoisonPotion*>(selectedItem) || dynamic_cast<ExplosionPotion*>(selectedItem)) {
                    player.useItem(itemChoice - 1, enemy);
                }
                else if (dynamic_cast<HealthPotion*>(selectedItem) || dynamic_cast<StrengthPotion*>(selectedItem)) {
                    player.useItem(itemChoice - 1, player);
                }
                else {
                    player.useItem(itemChoice - 1, player);
                }
            }
            else if (itemChoice == player.getInventory().size() + 1) {
                std::cout << "退出物品清單。\n";
                continue;
            }
            else {
                std::cout << "無效的選項。\n";
                continue;
            }
        }
        else {
            std::cout << "無效的動作。\n";
            continue;
        }

        if (!enemy.isAlive()) {
            std::cout << enemy.getName() << " 被擊敗了!\n" << std::endl;
            std::string enemyName = enemy.getName();

            bool isGoblinEnemy = (enemyName == "哥布林士兵" || enemyName == "哥布林弓箭手" || enemyName == "哥布林狂戰士" || enemyName.rfind("哥布林 (等級", 0) == 0);

            if (enemyName == "哥布林士兵") {
                handleItemDrop(player, "劍", new Weapon("劍", 15, 3, 2), "🗡️");
            }
            else if (enemyName == "哥布林弓箭手") {
                handleItemDrop(player, "短弓", new Bow("短弓", 12, 5, 2), "🏹");
            }
            else if (enemyName == "哥布林狂戰士") {
                handleItemDrop(player, "石斧", new Axe("石斧", 18, 4, 3), "🪓");
            }
            else if (isGoblinEnemy) {
                int potionDropChance = rand() % 100;
                if (potionDropChance < 30) {
                    handleItemDrop(player, "治療藥水", new HealthPotion("治療藥水", 20), "🧪");
                }
                else if (potionDropChance < 50) {
                    handleItemDrop(player, "毒藥水", new PoisonPotion("毒藥水", 3), "☠️");
                }
                else if (potionDropChance < 70) {
                    handleItemDrop(player, "強化藥水", new StrengthPotion("強化藥水", 5), "💪");
                }
                else if (potionDropChance < 90) {
                    handleItemDrop(player, "爆炸藥水", new ExplosionPotion("爆炸藥水", 40), "💥");
                }
                else {
                    std::cout << "敵人什麼也沒掉落。\n" << std::endl;
                }
            }
            else {
                handleItemDrop(player, "治療藥水", new HealthPotion("治療藥水", 20), "🧪");
            }
            player.gainExperience(4 + player.getLevelValue());
            break;
        }

        damage = enemy.getAttackValue();
        player.takeDamage(damage);

        std::cout << enemy.getName() << " 攻擊了玩家，造成了 " << damage << " 點傷害。" << std::endl;
        std::cout << player.getName() << " 的防禦力抵擋了 " << player.getDefense() << " 點傷害。\n";
        std::cout << std::endl << player.getName() << " 受到了 " << std::max(0, damage - player.getDefense()) << " 點傷害。\n" << std::endl;
    }

    if (!player.isAlive()) {
        std::cout << "GAME OVER。" << std::endl;
    }
}

void showMenu() {
    std::cout << "╔═══════════════════════╗" << std::endl;
    std::cout << "║         主選單        ║" << std::endl;
    std::cout << "╚═══════════════════════╝" << std::endl;
    std::cout << "1. 探索地下城\n2. 查看物品清單\n3. 查看裝備武器\n4. 卸下裝備\n5. 退出遊戲\n選擇一個選項: " << std::endl;
}