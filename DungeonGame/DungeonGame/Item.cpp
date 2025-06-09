#include "./Item.h"
#include "Character.h" // 由於 applyEffect 需要 Character 類別的完整定義，因此在此包含
#include <string>

using namespace std;

// Item 類別實現
Item::Item(std::string n, int v, int l) : name_(n), value_(v), level_(l) {}
HealthPotion::HealthPotion(string n, int v) : Potion(n, v) {}
PoisonPotion::PoisonPotion(string n, int v) : Potion(n, v) {}
StrengthPotion::StrengthPotion(string n, int v) : Potion(n, v) {}
ExplosionPotion::ExplosionPotion(string n, int v) : Potion(n, v) {}

void Item::display() const {
    std::cout << name_ << " (等級: " << level_ << ", 效果: " << value_ << ")\n";
}

bool Item::isWeapon() const {
    return false;
}

void Item::applyEffect(Character& target, Character& attacker) {
    // 預設無效果
}

std::string Item::getName() const { return name_; }
int Item::getValue() const { return value_; }
int Item::getLevel() const { return level_; }

Item::~Item() {}

// Weapon 類別實現
Weapon::Weapon(std::string n, int v, int d, int l) : Item(n, v, l), durability_(d) {}

void Weapon::use() {
    if (durability_ > 0) {
        durability_--;
    }
}

bool Weapon::isBroken() const {
    return durability_ == 0;
}

void Weapon::display() const {
    std::cout << getName() << " (等級: " << getLevel() << ", 耐久度: " << durability_ << ", 攻擊: " << getValue() << ")\n";
}

bool Weapon::isWeapon() const {
    return true;
}

int Weapon::getDurability() const { return durability_; }

// Bow 類別實現
Bow::Bow(std::string n, int v, int d, int l) : Weapon(n, v, d, l) {}

void Bow::display() const {
    std::cout << getName() << " (等級: " << getLevel() << ", 耐久度: " << getDurability() << ", 攻擊: " << getValue() << ", 武器類型: 弓)\n";
}

// Axe 類別實現
Axe::Axe(std::string n, int v, int d, int l) : Weapon(n, v, d, l) {}

void Axe::display() const {
    std::cout << getName() << " (等級: " << getLevel() << ", 耐久度: " << getDurability() << ", 攻擊: " << getValue() << ", 武器類型: 斧)\n";
}

// Potion 類別實現
Potion::Potion(std::string n, int v) : Item(n, v) {}

void Potion::display() const {
    std::cout << getName() << " (等級: " << getLevel() << ", 效果: " << getValue() << ")\n";
}

// HealthPotion 類別效果實現
void HealthPotion::applyEffect(Character& target, Character& attacker) {
    target.heal(getValue());
    std::cout << target.getName() << " 使用了 " << getName() << "，恢復了 " << getValue() << " 生命！\n";
}

// PoisonPotion 類別效果實現
void PoisonPotion::applyEffect(Character& target, Character& attacker) {
    // 毒藥水只能對敵人使用
    if (&target != &attacker) { // 確保目標不是使用者本身 (即敵人)
        target.applyPoison(getValue()); // value_ 是回合數
        std::cout << attacker.getName() << " 對 " << target.getName() << " 使用了 " << getName() << "，使敵人中毒 " << getValue() << " 回合！\n";
    }
    else {
        std::cout << attacker.getName() << " 毒藥水無法對玩家使用。\n";
    }
}

// StrengthPotion 類別效果實現
void StrengthPotion::applyEffect(Character& target, Character& attacker) {
    // 強化藥水只能對自己使用
    if (&target == &attacker) {
        target.applyStrength(getValue(), 3); // value_ 是攻擊力加成，固定持續 3 回合
        std::cout << target.getName() << " 使用了 " << getName() << "，攻擊力提升 " << getValue() << " 點，持續 3 回合！\n";
    }
    else {
        std::cout << attacker.getName() << " 強化藥水無法對敵人使用。\n";
    }
}

// ExplosionPotion 類別效果實現
void ExplosionPotion::applyEffect(Character& target, Character& attacker) {
    // 爆炸藥水對敵人造成傷害
    if (&target != &attacker) {
        int explosionDamage = getValue(); // value_ 是傷害值
        target.takeDamage(explosionDamage);
        std::cout << attacker.getName() << " 對 " << target.getName() << " 使用了 " << getName() << "，造成了 " << explosionDamage << " 點傷害！\n";
    }
    else {
        std::cout << attacker.getName() << " 爆炸藥水無法對玩家使用。\n";
    }
}