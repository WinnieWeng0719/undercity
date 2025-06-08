#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h"
#include <string>

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

class Bow : public Weapon {
public:
    Bow(std::string n, int v, int d, int l = 1);
    virtual void display() const override;
};

class Axe : public Weapon {
public:
    Axe(std::string n, int v, int d, int l = 1);
    virtual void display() const override;
};

#endif // WEAPON_H