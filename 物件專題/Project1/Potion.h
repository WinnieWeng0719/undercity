#ifndef POTION_H
#define POTION_H

#include "Item.h" // �~�Ӧ� Item
// �e�V�ŧi Character ���O�A�]�� Potion ���O�� applyEffect �禡�|�Ψ쥦
class Character;

// �Ĥ����O�A�~�Ӧ� Item
class Potion : public Item {
public:
    Potion(std::string n, int v);
    virtual void display() const override;

    // �Ҧ��Ĥ����� applyEffect�A����ĪG�b�l���O��{
    virtual void applyEffect(Character& target, Character& attacker) override = 0; // �]���µ����禡
};

// �v���Ĥ����O
class HealthPotion : public Potion {
public:
    HealthPotion(std::string n, int v);
    virtual void applyEffect(Character& target, Character& attacker) override;
};

// �s�W�r�Ĥ����O
class PoisonPotion : public Potion {
public:
    PoisonPotion(std::string n, int v); // value_�b�o�̪�ܬr������^�X��
    virtual void applyEffect(Character& target, Character& attacker) override;
};

// �s�W�j���Ĥ����O
class StrengthPotion : public Potion {
public:
    StrengthPotion(std::string n, int v); // value_�b�o�̪�ܧ����O�W�q��
    virtual void applyEffect(Character& target, Character& attacker) override;
};

// �s�W�z���Ĥ����O
class ExplosionPotion : public Potion {
public:
    ExplosionPotion(std::string n, int v); // value_�b�o�̪�ܶˮ`��
    virtual void applyEffect(Character& target, Character& attacker) override;
};

#endif // POTION_H