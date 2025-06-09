#ifndef POTION_H
#define POTION_H

#include "Item.h" // 繼承自 Item
// 前向宣告 Character 類別，因為 Potion 類別的 applyEffect 函式會用到它
class Character;

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

#endif // POTION_H