#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm> // for std::max

#include "Item.h"
#include "Weapon.h"
#include "Potion.h"
#include "Character.h"
#include "game_functions.h" // 包含遊戲相關函式

int main() {
    srand(time(0));

    Character player("玩家", 21, 10, 3);
    std::cout << "歡迎來到地下城" << std::endl;
    std::cout << std::endl << "玩家資訊" << std::endl <<
        "等級: " << player.getLevelValue() << std::endl << "攻擊力: " << player.getAttackValue() << " 生命值: " << player.getHealth() << " 防禦力: " << player.getDefense() << std::endl;

    int consecutiveNothingFound = 0;

    while (true) {
        showMenu();
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "#-------------#";
            std::cout << "\n玩家進入地下城..." << std::endl;

            std::cout << "\n玩家資訊" << std::endl;
            std::cout << "等級: " << player.getLevelValue() << std::endl
                << "攻擊力: " << player.getAttackValue()
                << " 生命值: " << player.getHealth()
                << " 防禦力: " << player.getDefense() << "\n" << std::endl;

            if (consecutiveNothingFound < 2) {
                if (rand() % 3 == 0) { // 33% 機率遇到敵人
                    Character enemy = generateEnemy(player.getLevelValue());
                    std::cout << "\n遇到敵人: " << enemy.getName() << std::endl;
                    battle(player, enemy);
                    consecutiveNothingFound = 0;
                }
                else {
                    std::cout << "地下城一片寧靜... \n" << std::endl;;
                    consecutiveNothingFound++;
                }
            }
            else {
                Character enemy = generateEnemy(player.getLevelValue());
                std::cout << "\n遇到敵人: " << enemy.getName() << std::endl;
                battle(player, enemy);
                consecutiveNothingFound = 0;
            }
        }
        else if (choice == 2) {
            while (true) {
                if (player.getInventory().empty()) {
                    std::cout << "\n物品清單是空的！\n" << std::endl;
                    break;
                }
                player.displayItemList();
                std::cout << "選擇一個選項: ";
                int itemChoice;
                std::cin >> itemChoice;
                if (itemChoice == player.getInventory().size() + 1) {
                    break;
                }
                else {
                    Item* selectedItem = player.getInventory()[itemChoice - 1];
                    if (dynamic_cast<PoisonPotion*>(selectedItem) || dynamic_cast<ExplosionPotion*>(selectedItem)) {
                        std::cout << "此藥水僅在戰鬥中使用！\n";
                    }
                    else {
                        player.useItem(itemChoice - 1, player);
                    }
                }
            }
        }
        else if (choice == 3) {
            player.displayEquippedWeapon();
        }
        else if (choice == 4) {
            player.unequipWeapon();
        }
        else if (choice == 5) {
            std::cout << "退出遊戲。期待再次挑戰地下城!\n";
            break;
        }
        else {
            std::cout << "無效的選項，請再試一次。\n";
        }

        if (!player.isAlive()) {
            break;
        }
    }

    return 0;
}