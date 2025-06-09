#include "./Item.h"
#include "Character.h" // 需要包含 Character.h 因為 applyEffect 會用到 Character 類別

// 物品類別實作
Item::Item(std::string n, int v, int l) : name_(n), value_(v), level_(l) {}

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