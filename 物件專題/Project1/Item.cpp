#include "Item.h"
#include "Character.h" // �ݭn�]�t Character.h �]�� applyEffect �|�Ψ� Character ���O

// ���~���O��@
Item::Item(std::string n, int v, int l) : name_(n), value_(v), level_(l) {}

void Item::display() const {
    std::cout << name_ << " (����: " << level_ << ", �ĪG: " << value_ << ")\n";
}

bool Item::isWeapon() const {
    return false;
}

void Item::applyEffect(Character& target, Character& attacker) {
    // �w�]�L�ĪG
}

std::string Item::getName() const { return name_; }
int Item::getValue() const { return value_; }
int Item::getLevel() const { return level_; }

Item::~Item() {}