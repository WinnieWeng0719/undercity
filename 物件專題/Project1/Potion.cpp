#include "Potion.h"
#include "Character.h" // �ݭn�]�t Character.h �]���Ĥ��ĪG�|�ާ@ Character

using namespace std;


// �Ĥ����O��@
Potion::Potion(std::string n, int v) : Item(n, v) {}
HealthPotion::HealthPotion(string n, int v) : Potion(n, v) {}
PoisonPotion::PoisonPotion(string n, int v) : Potion(n, v) {}
StrengthPotion::StrengthPotion(string n, int v) : Potion(n, v) {}
ExplosionPotion::ExplosionPotion(string n, int v) : Potion(n, v) {}

void Potion::display() const {
    std::cout << getName() << " (����: " << getLevel() << ", �ĪG: " << getValue() << ")\n";
}

// �v���Ĥ��ĪG��{
void HealthPotion::applyEffect(Character& target, Character& attacker) {
    target.heal(getValue());
    std::cout << target.getName() << " �ϥΤF " << getName() << "�A��_�F " << getValue() << " �ͩR�I\n";
}

// �r�Ĥ��ĪG��{
void PoisonPotion::applyEffect(Character& target, Character& attacker) {
    // �r�Ĥ��u���ĤH�ϥ�
    if (&target != &attacker) { // �T�O�ؼФ��O�ϥΪ̥��� (�Y�ĤH)
        target.applyPoison(getValue()); // value_ �O�^�X��
        std::cout << attacker.getName() << " �� " << target.getName() << " �ϥΤF " << getName() << "�A�ϼĤH���r " << getValue() << " �^�X�I\n";
    }
    else {
        std::cout << attacker.getName() << " �r�Ĥ��L�k�缾�a�ϥΡC\n";
    }
}

// �j���Ĥ��ĪG��{
void StrengthPotion::applyEffect(Character& target, Character& attacker) {
    // �j���Ĥ��u���ۤv�ϥ�
    if (&target == &attacker) {
        target.applyStrength(getValue(), 3); // value_ �O�����O�[���A�T�w���� 3 �^�X
        std::cout << target.getName() << " �ϥΤF " << getName() << "�A�����O���� " << getValue() << " �I�A���� 3 �^�X�I\n";
    }
    else {
        std::cout << attacker.getName() << " �j���Ĥ��L�k��ĤH�ϥΡC\n";
    }
}

// �z���Ĥ��ĪG��{
void ExplosionPotion::applyEffect(Character& target, Character& attacker) {
    // �z���Ĥ���ĤH�y���ˮ`
    if (&target != &attacker) {
        int explosionDamage = getValue(); // value_ �O�ˮ`��
        target.takeDamage(explosionDamage);
        std::cout << attacker.getName() << " �� " << target.getName() << " �ϥΤF " << getName() << "�A�y���F " << explosionDamage << " �I�ˮ`�I\n";
    }
    else {
        std::cout << attacker.getName() << " �z���Ĥ��L�k�缾�a�ϥΡC\n";
    }
}