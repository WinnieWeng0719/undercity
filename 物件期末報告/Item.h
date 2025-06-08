#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

// 前向宣告 Character 類別，避免循環引用
class Character;

// 物品類別
class Item {
private:
    std::string name_;
    int value_;    // 物品效果值
    int level_;    // 物品等級

public:
    Item(std::string n, int v, int l = 1);
    virtual void display() const;
    virtual bool isWeapon() const;
    virtual void applyEffect(Character& target, Character& attacker);

    // Getter 方法
    std::string getName() const;
    int getValue() const;
    int getLevel() const;

    // 虛擬析構函式，確保正確釋放記憶體
    virtual ~Item();
};

// 武器類別，繼承自 Item
class Weapon : public Item {
private:
    int durability_;    // 武器的耐久度

public:
    Weapon(std::string n, int v, int d, int l = 1);
    void use();
    bool isBroken() const;
    virtual void display() const override;
    virtual bool isWeapon() const override;

    // Getter 方法
    int getDurability() const;
};

// 新增弓類別，繼承自 Weapon
class Bow : public Weapon {
public:
    Bow(std::string n, int v, int d, int l = 1);
    virtual void display() const override;
};

// 新增斧頭類別，繼承自 Weapon
class Axe : public Weapon {
public:
    Axe(std::string n, int v, int d, int l = 1);
    virtual void display() const override;
};

// 藥水類別，繼承自 Item
class Potion : public Item {
public:
    Potion(std::string n, int v);
    virtual void display() const override;
    // 所有藥水都有 applyEffect，具體效果在子類別實現
    virtual void applyEffect(Character& target, Character& attacker) override = 0; // 設為純虛擬函式
};

// 治療藥水類別
class HealthPotion : public Potion {
public:
    HealthPotion(std::string n, int v);
    virtual void applyEffect(Character& target, Character& attacker) override;
};

// 新增毒藥水類別
class PoisonPotion : public Potion {
public:
    PoisonPotion(std::string n, int v); // value_在這裡表示毒素持續回合數
    virtual void applyEffect(Character& target, Character& attacker) override;
};

// 新增強化藥水類別
class StrengthPotion : public Potion {
public:
    StrengthPotion(std::string n, int v); // value_在這裡表示攻擊力增益值
    virtual void applyEffect(Character& target, Character& attacker) override;
};

// 新增爆炸藥水類別
class ExplosionPotion : public Potion {
public:
    ExplosionPotion(std::string n, int v); // value_在這裡表示傷害值
    virtual void applyEffect(Character& target, Character& attacker) override;
};

#endif // ITEM_H