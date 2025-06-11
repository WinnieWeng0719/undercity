#include "Potion.h"
#include "Character.h" // 需要包含 Character.h 因為藥水效果會操作 Character

using namespace std;


// 藥水類別實作
Potion::Potion(std::string n, int v) : Item(n, v) {}
HealthPotion::HealthPotion(string n, int v) : Potion(n, v) {}
PoisonPotion::PoisonPotion(string n, int v) : Potion(n, v) {}
StrengthPotion::StrengthPotion(string n, int v) : Potion(n, v) {}
ExplosionPotion::ExplosionPotion(string n, int v) : Potion(n, v) {}

void Potion::display() const {
    std::cout << getName() << " (等級: " << getLevel() << ", 效果: " << getValue() << ")\n";
}

// 治療藥水效果實現
void HealthPotion::applyEffect(Character& target, Character& attacker) {
    target.heal(getValue());
    std::cout << target.getName() << " 使用了 " << getName() << "，恢復了 " << getValue() << " 生命！\n";
}

// 毒藥水效果實現
void PoisonPotion::applyEffect(Character& target, Character& attacker) {
    // 毒藥水只能對敵人使用
    if (&target != &attacker) { // 確保目標不是使用者本身 (即敵人)
        target.applyPoison(getValue()); // value_ 是回合數
        std::cout << attacker.getName() << " 對 " << target.getName() << " 使用了 " << getName() << "，使敵人中毒 " << getValue() << " 回合！\n";
    }
    else {
        std::cout << attacker.getName() << " 毒藥水無法對玩家使用。\n";
    }
}

// 強化藥水效果實現
void StrengthPotion::applyEffect(Character& target, Character& attacker) {
    // 強化藥水只能對自己使用
    if (&target == &attacker) {
        target.applyStrength(getValue(), 3); // value_ 是攻擊力加成，固定持續 3 回合
        std::cout << target.getName() << " 使用了 " << getName() << "，攻擊力提升 " << getValue() << " 點，持續 3 回合！\n";
    }
    else {
        std::cout << attacker.getName() << " 強化藥水無法對敵人使用。\n";
    }
}

// 爆炸藥水效果實現
void ExplosionPotion::applyEffect(Character& target, Character& attacker) {
    // 爆炸藥水對敵人造成傷害
    if (&target != &attacker) {
        int explosionDamage = getValue(); // value_ 是傷害值
        target.takeDamage(explosionDamage);
        std::cout << attacker.getName() << " 對 " << target.getName() << " 使用了 " << getName() << "，造成了 " << explosionDamage << " 點傷害！\n";
    }
    else {
        std::cout << attacker.getName() << " 爆炸藥水無法對玩家使用。\n";
    }
}