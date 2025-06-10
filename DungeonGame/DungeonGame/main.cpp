#include <iostream>
#include <ctime>   // For time()
#include <cstdlib> // For srand()
#include <string>

#include "Character.h"       // 引入 Character 類別
#include "./Item.h" // 確保包含檔案的路徑正確           // 引入 Item 類別及其子類別
#include "./game_funtion.h"  // 引入遊戲函式
using namespace std;
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

            // 顯示玩家資訊
            std::cout << "\n玩家資訊" << std::endl;
            std::cout << "等級: " << player.getLevelValue() << std::endl
                << "攻擊力: " << player.getAttackValue()
                << " 生命值: " << player.getHealth()
                << " 防禦力: " << player.getDefense() << "\n" << std::endl;


            if (consecutiveNothingFound < 2) {
                if (rand() % 3 == 0) { // 33% 機率遇到敵人
                    Character enemy = generateEnemy(player.getLevelValue());
                    std::cout << "\n遇到敵人: " << enemy.getName() << std::endl;
                    bool enemyDefeated = battle(player, enemy); // 呼叫戰鬥，並接收結果
                    consecutiveNothingFound = 0;

                    // 判斷是否打敗的是 "你的作業"
                    if (enemy.getName() == "你的作業" && !enemy.isAlive()) {
                    cout << "\n🎉 恭喜玩家擊敗了最硬敵人『你的作業』！玩家已經完成了地下城的最大挑戰！" << endl;
                    cout << "🏆 地下城遊戲結束，感謝玩家的挑戰！\n" << endl;
                    cout << "玩家離開地下城" << endl;
                    break; // 結束主遊戲迴圈
                }
                else {
                    std::cout << "地下城一片寧靜... \n" << std::endl;;
                    consecutiveNothingFound++;
                }
            }
            else {
                // 連續兩次什麼都沒找到，這次強制遇到敵人
                Character enemy = generateEnemy(player.getLevelValue());
                std::cout << "\n遇到敵人: " << enemy.getName() << std::endl;
                 bool enemyDefeated = battle(player, enemy); // 呼叫戰鬥，並接收結果
                consecutiveNothingFound = 0;

                // 判斷是否打敗的是 "你的作業"
                if (enemy.getName() == "你的作業" && !enemy.isAlive()) {
                    cout << "\n🎉 恭喜玩家擊敗了最硬敵人『你的作業』！玩家已經完成了地下城的最大挑戰！" << endl;
                    cout << "🏆 地下城遊戲結束，感謝玩家的挑戰！\n" << endl;
                    cout << "玩家離開地下城" << endl;
                    break; // 結束主遊戲迴圈
                }
            }
        }
        else if (choice == 2) {
            while (true) {
                if (player.getInventory().empty()) {
                    std::cout << "\n物品清單是空的！\n" << std::endl;
                    break; // 如果背包空了，就直接退出循環
                }
                player.displayItemList();
                std::cout << "選擇一個選項: ";
                int itemChoice;
                std::cin >> itemChoice;
                if (itemChoice == player.getInventory().size() + 1) {
                    break;
                }
                else {
                    // 使用物品時，需要判斷目標
                    Item* selectedItem = player.getInventory()[itemChoice - 1];
                    if (dynamic_cast<PoisonPotion*>(selectedItem) || dynamic_cast<ExplosionPotion*>(selectedItem)) {
                        std::cout << "此藥水僅在戰鬥中使用！\n"; // 這些藥水只能在戰鬥中對敵人使用
                    }
                    else {
                        player.useItem(itemChoice - 1, player); // 對自己使用
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