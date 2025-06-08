#ifndef GAME_FUNCTIONS_H
#define GAME_FUNCTIONS_H

#include "Character.h"
#include "Item.h" // For Item* in handleItemDrop
#include <string>

Character generateEnemy(int playerLevel);
void handleItemDrop(Character& player, const std::string& itemName, Item* droppedItem, const std::string& icon);
void battle(Character& player, Character& enemy);
void showMenu();

#endif // GAME_FUNCTIONS_H