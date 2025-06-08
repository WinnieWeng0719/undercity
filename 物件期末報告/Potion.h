#ifndef POTION_H
#define POTION_H

#include "Item.h"
#include <string>

class Potion : public Item {
public:
    Potion(std::string n, int v);
    virtual void display() const override;
    virtual void applyEffect(class Character& target, class Character& attacker) override = 0; // 純虛擬函式
};

class HealthPotion : public Potion {
public:
    HealthPotion(std::string n, int v);
    virtual void applyEffect(class Character& target, class Character& attacker) override;
};

class PoisonPotion : public Potion {
public:
    PoisonPotion(std::string n, int v);
    virtual void applyEffect(class Character& target, class Character& attacker) override;
};

class StrengthPotion : public Potion {
public:
    StrengthPotion(std::string n, int v);
    virtual void applyEffect(class Character& target, class Character& attacker) override;
};

class ExplosionPotion : public Potion {
public:
    ExplosionPotion(std::string n, int v);
    virtual void applyEffect(class Character& target, class Character& attacker) override;
};

#endif // POTION_H