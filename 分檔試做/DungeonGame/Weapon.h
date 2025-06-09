#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h" // 繼承自 Item

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

#endif // WEAPON_H