#include "game_funtion.h"
#include <iostream>
#include <cstdlib> // For rand(), srand()
#include <ctime>   // For time()
#include <sstream> // For to_string (though <string> usually suffices)

// 隨機生成敵人
Character generateEnemy(int playerLevel) {
    int enemyType = rand() % 100;

    if (playerLevel >= 20) {
        if (enemyType < 30) {
            return Character("你的作業", 500, rand() % 40 + 100, 10);
        }
    }

    if (playerLevel >= 12) {
        if (enemyType < 40) {
            return Character("宇宙無敵超級深淵惡魔-龍龍", 250, rand() % 4 + 200, 20);
        }
    }

    if (playerLevel >= 9) {
        if (enemyType < 40) {
            return Character("林克", 100, rand() % 25 + 80, 15);
        }
    }

    if (playerLevel >= 6) {
        if (enemyType < 50) {
            return Character("芙莉蓮花", 50, rand() % 10 + 60, 5);
        }
    }

    // 新增：確保最低等級玩家只會遇到哥布林系
    if (enemyType < 60) {
        int level = rand() % 3 + 1;
        int health = (level == 1) ? 12 : (level == 2) ? 15 : 17;
        return Character("哥布林 (等級 " + std::to_string(level) + ")", health, rand() % 3 + 8, 1);
    }
    else if (enemyType < 70) {
        return Character("哥布林士兵", 30, rand() % 4 + 10, 3);
    }
    else if (enemyType < 80) {
        return Character("哥布林弓箭手", 25, rand() % 5 + 12, 2);
    }
    else if (enemyType < 90) {
        return Character("哥布林狂戰士", 35, rand() % 6 + 18, 4);
    }

    // 預設安全 fallback
    int level = rand() % 3 + 1;
    int health = (level == 1) ? 12 : (level == 2) ? 15 : 17;
    return Character("哥布林 (等級 " + std::to_string(level) + ")", health, rand() % 3 + 8, 1);
}

// 新增處理物品掉落的函式
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
            delete droppedItem; // 如果不撿取，就刪除這個物品
            validInput = true;
        }
        else {
            std::cout << "無效選擇，請重新輸入。\n";
        }
    }
}

// 戰鬥系統
void battle(Character& player, Character& enemy) {
    std::cout << "一隻 " << enemy.getName() << " 出現了！\n" << std::endl;

    while (player.isAlive() && enemy.isAlive()) {

        player.decreaseStrengthTurns(); // 減少玩家強化回合數
        enemy.takePoisonDamage();       // 敵人中毒扣血

        // 顯示雙方血量和狀態
        player.displayStats();
        enemy.displayStats();

        // 如果敵人因毒素死亡，則直接結束戰鬥
        if (!enemy.isAlive()) {
            std::cout << enemy.getName() << " 因中毒而死亡！\n" << std::endl;
            player.gainExperience(4 + player.getLevelValue());
            break;
        }

        // 新增敵人血量高於玩家時的警告
        if (enemy.getHealth() > player.getHealth()) {
            std::cout << "⚠️ 警告：血量過低!請小心應戰！\n";
        }

        // 顯示攻擊選項
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
            damage = player.getAttack(); // 使用 getAttack() 包含武器和強化效果
            std::cout << player.getName() << " 選擇 [普通攻擊] ，造成了 " << damage << " 點傷害。\n";
            std::cout << enemy.getName() << " 的防禦力為他抵擋了 " << enemy.getDefense() << " 點傷害。\n";
            std::cout << std::endl << enemy.getName() << " 受到了 " << std::max(0, damage - enemy.getDefense()) << " 點傷害。\n" << std::endl;
            enemy.takeDamage(damage);
        }
        else if (action == 2) {
            damage = player.getAttack() * 2; // 使用 getAttack() 包含武器和強化效果
            std::cout << player.getName() << " 選擇 [強力攻擊] ，造成了 " << damage << " 點傷害。\n";
            std::cout << enemy.getName() << " 的防禦力抵擋了 " << enemy.getDefense() << " 點傷害。\n";
            std::cout << std::endl << enemy.getName() << " 受到了 " << std::max(0, damage - enemy.getDefense()) << " 點傷害。\n" << std::endl;
            enemy.takeDamage(damage);
        }
        else if (action == 3) {
            if (player.getEquippedWeapon() != nullptr) {
                player.getEquippedWeapon()->use();
                damage = 2 * player.getAttack() + (0.5 * player.getEquippedWeapon()->getValue()); // 使用 getAttack() 包含武器和強化效果
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
                // 根據物品類型，決定對誰使用
                Item* selectedItem = player.getInventory()[itemChoice - 1];
                bool isExplosionPotionUsed = false; // 標記是否使用了爆炸藥水

                if (dynamic_cast<PoisonPotion*>(selectedItem) || dynamic_cast<ExplosionPotion*>(selectedItem)) {
                    // 毒藥水和爆炸藥水用於敵人
                    if (dynamic_cast<ExplosionPotion*>(selectedItem)) {
                        isExplosionPotionUsed = true; // 如果是爆炸藥水，設定標記
                    }
                    player.useItem(itemChoice - 1, enemy);
                }
                else if (dynamic_cast<HealthPotion*>(selectedItem) || dynamic_cast<StrengthPotion*>(selectedItem)) {
                    // 治療藥水和強化藥水用於自己
                    player.useItem(itemChoice - 1, player);
                }
                else {
                    // 其他物品（如裝備）的預設處理
                    player.useItem(itemChoice - 1, player); // 對自己使用 (武器裝備)
                }

                // 處理爆炸藥水導致敵人死亡的邏輯
                if (isExplosionPotionUsed && !enemy.isAlive()) {
                    std::cout << enemy.getName() << " 被擊敗了!\n" << std::endl;
                    // 根據敵人掉落不同的物品 (這部分保留原有的掉落邏輯)
                    std::string enemyName = enemy.getName();

                    // 判斷是否為哥布林類型的敵人
                    bool isGoblinEnemy = (enemyName == "哥布林士兵" || enemyName == "哥布林弓箭手" || enemyName == "哥布林狂戰士" || enemyName.rfind("哥布林 (等級", 0) == 0);

                    if (enemyName == "哥布林士兵") {
                        handleItemDrop(player, "劍", new Weapon("劍", 15, 3, 2), "🗡️");
                    }
                    else if (enemyName == "哥布林弓箭手") {
                        handleItemDrop(player, "短弓", new Bow("短弓", 12, 5, 2), "🏹");
                    }
                    else if (enemyName == "哥布林狂戰士") {
                        handleItemDrop(player, "石斧", new Axe("石斧", 18, 2, 3), "🪓");
                    }
                    // 哥布林敵人有機會掉落藥水
                    else if (isGoblinEnemy) {
                        int potionDropChance = rand() % 100;

                        if (potionDropChance < 35) { //  治療藥水：35%
                            handleItemDrop(player, "治療藥水", new HealthPotion("治療藥水", 20), "🧪");
                        }
                        else if (potionDropChance < 55) { // 毒藥水：20%
                            handleItemDrop(player, "毒藥水", new PoisonPotion("毒藥水", 3), "☠️");
                        }
                        else if (potionDropChance < 75) { // 強化藥水：20%
                            handleItemDrop(player, "強化藥水", new StrengthPotion("強化藥水", 5), "💪");
                        }
                        else if (potionDropChance < 95) { // 爆炸藥水：20%
                            handleItemDrop(player, "爆炸藥水", new ExplosionPotion("爆炸藥水", 40), "💥");
                        }
                        else { // 剩下 5%：無掉落
                            std::cout << "敵人什麼也沒掉落。\n" << std::endl;
                        }
                    }

                    else { // 其他非哥布林敵人，預設只掉落治療藥水
                        handleItemDrop(player, "治療藥水", new HealthPotion("治療藥水", 20), "🧪");
                    }
                    player.gainExperience(4 + player.getLevelValue());
                    break; // 敵人死亡，結束戰鬥
                }
                else {
                    continue; // 如果使用其他物品或者爆炸藥水未殺死敵人，則繼續戰鬥
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
            // 根據敵人掉落不同的物品
            std::string enemyName = enemy.getName();

            // 判斷是否為哥布林類型的敵人
            bool isGoblinEnemy = (enemyName == "哥布林士兵" || enemyName == "哥布林弓箭手" || enemyName == "哥布林狂戰士" || enemyName.rfind("哥布林 (等級", 0) == 0);

            if (enemyName == "哥布林士兵") {
                handleItemDrop(player, "劍", new Weapon("劍", 15, 3, 2), "🗡️");
            }
            else if (enemyName == "哥布林弓箭手") {
                handleItemDrop(player, "短弓", new Bow("短弓", 12, 5, 2), "🏹");
            }
            else if (enemyName == "哥布林狂戰士") {
                handleItemDrop(player, "石斧", new Axe("石斧", 18, 2, 3), "🪓");
            }
            // 哥布林敵人有機會掉落藥水
            else if (isGoblinEnemy) {
                int potionDropChance = rand() % 100;
                if (potionDropChance < 35) { // 35% 機率掉落治療藥水
                    handleItemDrop(player, "治療藥水", new HealthPotion("治療藥水", 20), "🧪");
                }
                else if (potionDropChance < 55) { // 20% 機率掉落毒藥水 (累積機率 35+20=55%)
                    handleItemDrop(player, "毒藥水", new PoisonPotion("毒藥水", 3), "☠️"); // 持續 3 回合
                }
                else if (potionDropChance < 75) { // 20% 機率掉落強化藥水 (累積機率 55+20=75%)
                    handleItemDrop(player, "強化藥水", new StrengthPotion("強化藥水", 5), "💪"); // 攻擊力加成 5
                }
                else if (potionDropChance < 95) { // 20% 機率掉落爆炸藥水 (累積機率 75+20=95%)
                    handleItemDrop(player, "爆炸藥水", new ExplosionPotion("爆炸藥水", 40), "💥"); // 造成 40 點傷害
                }
                else { // 5% 機率什麼都沒掉落 (累積機率 95+5=100%)
                    std::cout << "敵人什麼也沒掉落。\n" << std::endl;
                }
            }
            else { // 其他非哥布林敵人，預設只掉落治療藥水
                handleItemDrop(player, "治療藥水", new HealthPotion("治療藥水", 20), "🧪");
            }
            player.gainExperience(4 + player.getLevelValue());
            break;
        }

        // 敵人回擊
        damage = enemy.getAttackValue();
        player.takeDamage(damage);

        std::cout << enemy.getName() << " 攻擊了玩家，造成了 " << damage << " 點傷害。" << std::endl;
        std::cout << player.getName() << " 的防禦力抵擋了 " << player.getDefense() << " 點傷害。\n";
        std::cout << std::endl << player.getName() << " 受到了 " << std::max(0, damage - player.getDefense()) << " 點傷害。\n" << std::endl;

    }

    if (!player.isAlive()) {
        std::cout << std::endl << "GAME OVER。" << std::endl;
    }
}

// 主選單
void showMenu() {
    std::cout << "╔═══════════════════════╗" << std::endl;
    std::cout << "║         主選單        ║" << std::endl;
    std::cout << "╚═══════════════════════╝" << std::endl;
    std::cout << "1. 探索地下城\n2. 查看物品清單\n3. 查看裝備武器\n4. 卸下裝備\n5. 退出遊戲\n選擇一個選項: " << std::endl;
}