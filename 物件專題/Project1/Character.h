#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <algorithm> // for std::max

// �e�V�ŧi Item �M Weapon ���O�A�קK�`���ޥ�
class Item;
class Weapon;

// �������O
class Character {
private:
    std::string name_;
    int health_;
    int attack_;
    int defense_;
    int experience_;    // �s�W�g����ݩ�
    int level_;
    std::vector<Item*> inventory_;
    Weapon* equippedWeapon_;    // �˳ƪ��Z��

    // �s�W���A�ܼ�
    int poisonTurns_;   // ���r�Ѿl�^�X��
    int strengthTurns_; // �j�ƳѾl�^�X��
    int strengthBonus_; // �j�ƴ��Ѫ������O�[��

public:
    Character(std::string n, int h, int a, int d);
    ~Character();

    void takeDamage(int damage);
    void heal(int amount);
    bool isAlive() const;
    void addItem(Item* item);
    void displayStats() const;
    int getAttack() const;
    void displayItemList() const;
    void useItem(int index, Character& targetCharacter);
    void displayEquippedWeapon() const;
    void gainExperience(int exp);
    void levelUp();

    std::string getName() const;
    int getHealth() const;
    int getAttackValue() const;
    int getDefense() const;
    int getLevelValue() const;
    Weapon* getEquippedWeapon() const;
    const std::vector<Item*>& getInventory() const;

    void unequipWeapon(bool silent = false);
    void removeItemFromInventory(int index);

    // �s�W�r��������k
    void applyPoison(int turns);
    void takePoisonDamage();

    // �s�W�j���Ĥ�������k
    void applyStrength(int bonus, int turns);
    void decreaseStrengthTurns();
};

#endif // CHARACTER_H