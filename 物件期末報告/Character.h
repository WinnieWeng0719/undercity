#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <algorithm> // for std::max

// 前向宣告 Item 和 Weapon 類別，避免循環引用
class Item;
class Weapon;

// 角色類別
class Character {
private:
    std::string name_;
    int health_;
    int attack_;
    int defense_;
    int experience_;    // 新增經驗值屬性
    int level_;
    std::vector<Item*> inventory_;
    Weapon* equippedWeapon_;    // 裝備的武器

    // 新增狀態變數
    int poisonTurns_;   // 中毒剩餘回合數
    int strengthTurns_; // 強化剩餘回合數
    int strengthBonus_; // 強化提供的攻擊力加成

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

    // 新增毒素相關方法
    void applyPoison(int turns);
    void takePoisonDamage();

    // 新增強化藥水相關方法
    void applyStrength(int bonus, int turns);
    void decreaseStrengthTurns();
};

#endif // CHARACTER_H