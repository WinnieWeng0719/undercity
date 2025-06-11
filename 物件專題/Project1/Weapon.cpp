#include "Weapon.h"

// �Z�����O��@
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
    std::cout << getName() << " (����: " << getLevel() << ", �@�[��: " << durability_ << ", ����: " << getValue() << ")\n";
}

bool Weapon::isWeapon() const {
    return true;
}

int Weapon::getDurability() const { return durability_; }

// �}���O��@
Bow::Bow(std::string n, int v, int d, int l) : Weapon(n, v, d, l) {}

void Bow::display() const {
    std::cout << getName() << " (����: " << getLevel() << ", �@�[��: " << getDurability() << ", ����: " << getValue() << ", �Z������: �})\n";
}

// ���Y���O��@
Axe::Axe(std::string n, int v, int d, int l) : Weapon(n, v, d, l) {}

void Axe::display() const {
    std::cout << getName() << " (����: " << getLevel() << ", �@�[��: " << getDurability() << ", ����: " << getValue() << ", �Z������: ��)\n";
}