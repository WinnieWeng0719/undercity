#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h" // �~�Ӧ� Item

// �Z�����O�A�~�Ӧ� Item
class Weapon : public Item {
private:
    int durability_;    // �Z�����@�[��

public:
    Weapon(std::string n, int v, int d, int l = 1);
    void use();
    bool isBroken() const;
    virtual void display() const override;
    virtual bool isWeapon() const override;

    // Getter ��k
    int getDurability() const;
};

// �s�W�}���O�A�~�Ӧ� Weapon
class Bow : public Weapon {
public:
    Bow(std::string n, int v, int d, int l = 1);
    virtual void display() const override;
};

// �s�W���Y���O�A�~�Ӧ� Weapon
class Axe : public Weapon {
public:
    Axe(std::string n, int v, int d, int l = 1);
    virtual void display() const override;
};

#endif // WEAPON_H