#include "Weapon.h"
#include <iostream>

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

Bow::Bow(std::string n, int v, int d, int l) : Weapon(n, v, d, l) {}

void Bow::display() const {
    std::cout << getName() << " (等級: " << getLevel() << ", 耐久度: " << getDurability() << ", 攻擊: " << getValue() << ", 武器類型: 弓)\n";
}

Axe::Axe(std::string n, int v, int d, int l) : Weapon(n, v, d, l) {}

void Axe::display() const {
    std::cout << getName() << " (等級: " << getLevel() << ", 耐久度: " << getDurability() << ", 攻擊: " << getValue() << ", 武器類型: 斧)\n";
}