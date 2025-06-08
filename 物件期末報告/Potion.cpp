#include "Potion.h"
#include "Character.h" // 需要包含 Character.h 因為 applyEffect 函式需要 Character 物件
#include <iostream> // For std::cout
#include <string>
using namespace std;

Potion::Potion(std::string n, int v) : Item(n, v) {}
HealthPotion::HealthPotion(string n, int v) : Potion(n, v) {}
PoisonPotion::PoisonPotion(string n, int v) : Potion(n, v) {}
StrengthPotion::StrengthPotion(string n, int v) : Potion(n, v) {}
ExplosionPotion::ExplosionPotion(string n, int v) : Potion(n, v) {}


void Potion::display() const {
    std::cout << getName() << " (等級: " << getLevel() << ", 效果: " << getValue() << ")\n";
}

void HealthPotion::applyEffect(Character& target, Character& attacker) {
    target.heal(getValue());
    std::cout << target.getName() << " 使用了 " << getName() << "，恢復了 " << getValue() << " 生命！\n";
}

void PoisonPotion::applyEffect(Character& target, Character& attacker) {
    if (&target != &attacker) {
        target.applyPoison(getValue());
        std::cout << attacker.getName() << " 對 " << target.getName() << " 使用了 " << getName() << "，使敵人中毒 " << getValue() << " 回合！\n";
    }
    else {
        std::cout << attacker.getName() << " 毒藥水無法對玩家使用。\n";
    }
}

void StrengthPotion::applyEffect(Character& target, Character& attacker) {
    if (&target == &attacker) {
        target.applyStrength(getValue(), 3);
        std::cout << target.getName() << " 使用了 " << getName() << "，攻擊力提升 " << getValue() << " 點，持續 3 回合！\n";
    }
    else {
        std::cout << attacker.getName() << " 強化藥水無法對敵人使用。\n";
    }
}

void ExplosionPotion::applyEffect(Character& target, Character& attacker) {
    if (&target != &attacker) {
        int explosionDamage = getValue();
        target.takeDamage(explosionDamage);
        std::cout << attacker.getName() << " 對 " << target.getName() << " 使用了 " << getName() << "，造成了 " << explosionDamage << " 點傷害！\n";
    }
    else {
        std::cout << attacker.getName() << " 爆炸藥水無法對玩家使用。\n";
    }
}