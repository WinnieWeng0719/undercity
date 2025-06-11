#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

// �e�V�ŧi Character ���O�A�]�� Item ���O�� applyEffect �禡�|�Ψ쥦
class Character;

// ���~���O
class Item {
private:
    std::string name_;
    int value_;    // ���~�ĪG��
    int level_;    // ���~����

public:
    Item(std::string n, int v, int l = 1);
    virtual void display() const;
    virtual bool isWeapon() const;

    // �s�W�����禡�����Ϊ��~�ĪG�A�ݭn�ǤJ�ؼШ���M������
    // �w�]��������ơA�Ѥl���O�мg
    virtual void applyEffect(Character& target, Character& attacker);

    // Getter ��k
    std::string getName() const;
    int getValue() const;
    int getLevel() const;

    // �����R�c�禡�A�T�O���T����O����
    virtual ~Item();
};

#endif // ITEM_H