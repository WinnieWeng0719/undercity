#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm> // for std::max

// 前向宣告 Item 和 Weapon 類別
class Item;
class Weapon;

class Character {
private:
    std::string name_;
    int health_;
    int attack_;
    int defense_;
    int experience_;
    int level_;
    std::vector<Item*> inventory_;
    Weapon* equippedWeapon_;

    int poisonTurns_;
    int strengthTurns_;
    int strengthBonus_;

public:
    Character(std::string n, int h, int a, int d);
    ~Character();

    void takeDamage(int damage);
    void heal(int amount);
    bool isAlive() const;
    void addItem(Item* item);
    void displayStats() const;
    int getAttack() const;
    void displayItemList() const;
    void useItem(int index, Character& targetCharacter);
    void displayEquippedWeapon() const;
    void gainExperience(int exp);
    void levelUp();

    std::string getName() const;
    int getHealth() const;
    int getAttackValue() const;
    int getDefense() const;
    int getLevelValue() const;
    Weapon* getEquippedWeapon() const;
    const std::vector<Item*>& getInventory() const;

    void unequipWeapon(bool silent = false);
    void removeItemFromInventory(int index);

    void applyPoison(int turns);
    void takePoisonDamage();
    void applyStrength(int bonus, int turns);
    void decreaseStrengthTurns();
};

#endif // CHARACTER_H