#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include <string>
#include "Character.h" // 需要 Character 類別
#include "./Item.h"      // 需要 Item 類別及其子類別
#include "./Weapon.h" 
#include "./Potion.h"

// 隨機生成敵人
Character generateEnemy(int playerLevel);

// 新增處理物品掉落的函式
void handleItemDrop(Character& player, const std::string& itemName, Item* droppedItem, const std::string& icon);

// 戰鬥系統
bool battle(Character& player, Character& enemy);

// 主選單
void showMenu();

#endif // GAME_FUNCTIONS_H