#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include <string>
#include "Character.h" // �ݭn Character ���O
#include "Item.h"      // �ݭn Item ���O�Ψ�l���O
#include "Weapon.h" 
#include "Potion.h"

// �H���ͦ��ĤH
Character generateEnemy(int playerLevel);

// �s�W�B�z���~�������禡
void handleItemDrop(Character& player, const std::string& itemName, Item* droppedItem, const std::string& icon);

// �԰��t��
void battle(Character& player, Character& enemy);

// �D���
void showMenu();

#endif // GAME_FUNCTIONS_H