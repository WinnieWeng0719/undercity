#include "Character.h"
#include "Item.h"    // 需要 Item 類別的完整定義
// 注意：這裡不需要額外包含 Weapon.h 或 Potion.h，因為 Item.h 已經包含了它們的宣告，
// 且 Character.cpp 中對 Weapon 和 Potion 的操作都是透過 Item 指標和 dynamic_cast 進行的。

Character::Character(std::string n, int h, int a, int d)
    : name_(n), health_(h), attack_(a), defense_(d), experience_(0), level_(1), equippedWeapon_(nullptr),
    poisonTurns_(0), strengthTurns_(0), strengthBonus_(0) {
}

Character::~Character() {
    for (Item* item : inventory_) {
        delete item;
    }
    inventory_.clear();
    equippedWeapon_ = nullptr; // 確保指標為空
}

void Character::takeDamage(int damage) {
    health_ -= std::max(0, damage - defense_);
}

void Character::heal(int amount) {
    health_ += amount;
}

bool Character::isAlive() const {
    return health_ > 0;
}

void Character::addItem(Item* item) {
    inventory_.push_back(item);
}

void Character::displayStats() const {
    std::cout << name_ << " - 目前血量: " << health_ << " 攻擊：" << attack_;
    if (strengthTurns_ > 0) {
        std::cout << "狀態 :" << std::endl << " 強化中，剩餘(" << strengthTurns_ << "回合)";
    }
    if (poisonTurns_ > 0) {
        std::cout << "狀態 :" << std::endl << " 中毒中，剩餘(" << poisonTurns_ << "回合)";
    }
    std::cout << std::endl;
}

int Character::getAttack() const {
    return attack_ + (equippedWeapon_ ? (0.5 * equippedWeapon_->getValue()) : 0) + strengthBonus_;
}

void Character::displayItemList() const {
    std::cout << "\n#------------------#";
    std::cout << "\n|    物品清單      |" << std::endl;
    std::cout << "#------------------#" << std::endl << std::endl;
    if (inventory_.empty()) {
        std::cout << "物品清單是空的！\n";
    }
    else {
        for (int i = 0; i < inventory_.size(); ++i) {
            std::cout << i + 1 << ". ";
            inventory_[i]->display();
        }
    }
    std::cout << inventory_.size() + 1 << ". 退出物品清單\n" << std::endl;
}

void Character::useItem(int index, Character& targetCharacter) {
    if (index >= 0 && index < inventory_.size()) {
        Item* item = inventory_[index];
        if (item->isWeapon()) {
            Weapon* weapon = dynamic_cast<Weapon*>(item);
            if (weapon) {
                if (!weapon->isBroken()) {
                    if (equippedWeapon_ != nullptr) {
                        std::cout << "玩家已裝備了 " << equippedWeapon_->getName() << "，請先卸下才能裝備其他武器。\n";
                    }
                    else {
                        equippedWeapon_ = weapon;
                        std::cout << name_ << " 裝備了 " << weapon->getName() << "，攻擊力提升了 " << weapon->getValue() << "！\n";
                        inventory_.erase(inventory_.begin() + index);
                    }
                }
                else {
                    std::cout << weapon->getName() << " 已損毀，無法使用。\n";
                }
            }
        }
        else { // 如果是藥水，則應用效果
            Potion* potion = dynamic_cast<Potion*>(item);
            if (potion) {
                potion->applyEffect(targetCharacter, *this); // 傳入目標角色和玩家自己
                inventory_.erase(inventory_.begin() + index);
                delete item; // 釋放藥水記憶體
            }
        }
    }
    else {
        std::cout << "無效的物品。\n";
        std::cout << std::endl;
    }
}

void Character::displayEquippedWeapon() const {
    if (equippedWeapon_ != nullptr) {
        equippedWeapon_->display();
    }
    else {
        std::cout << "目前無裝備武器。\n";
    }
}

void Character::gainExperience(int exp) {
    experience_ += exp;
    std::cout << "增加了 " << exp << " 點經驗值！" << std::endl << std::endl;
    int experienceToNextLevel = level_ * 10;
    if (experience_ >= experienceToNextLevel) {
        experience_ -= experienceToNextLevel;
        levelUp();
    }
}

void Character::levelUp() {
    level_++;
    attack_ += 3;
    health_ += 5;
    defense_ += 2;
    std::cout << name_ << " 等級上升了！目前等級: " << level_ << std::endl;
    std::cout << "攻擊力: " << attack_ << " 生命值: " << health_ << " 防禦力: " << defense_ << std::endl << std::endl;
}

std::string Character::getName() const { return name_; }
int Character::getHealth() const { return health_; }
int Character::getAttackValue() const { return attack_; }
int Character::getDefense() const { return defense_; }
int Character::getLevelValue() const { return level_; }
Weapon* Character::getEquippedWeapon() const { return equippedWeapon_; }
const std::vector<Item*>& Character::getInventory() const { return inventory_; }

void Character::unequipWeapon(bool silent) {
    if (equippedWeapon_ != nullptr) {
        if (!silent) {
            inventory_.push_back(equippedWeapon_);
            std::cout << equippedWeapon_->getName() << " 已卸下並放回背包。\n";
        }
        else {
            delete equippedWeapon_;
        }
        equippedWeapon_ = nullptr;
    }
    else {
        if (!silent) {
            std::cout << "無裝備武器。\n";
        }
    }
}

void Character::removeItemFromInventory(int index) {
    if (index >= 0 && index < inventory_.size()) {
        if (inventory_[index] == equippedWeapon_) {
            equippedWeapon_ = nullptr;
        }
        delete inventory_[index];
        inventory_.erase(inventory_.begin() + index);
    }
}

void Character::applyPoison(int turns) {
    if (poisonTurns_ == 0) { // 如果沒有中毒，則施加中毒效果
        std::cout << name_ << " 中毒了！\n";
    }
    poisonTurns_ = turns;
}

void Character::takePoisonDamage() {
    if (poisonTurns_ > 0) {
        int poisonDmg = 5; // 每回合毒素傷害
        health_ -= poisonDmg;
        std::cout << name_ << " 因中毒減少了 " << poisonDmg << " 點生命！\n";
        poisonTurns_--;
        if (poisonTurns_ == 0) {
            std::cout << name_ << " 的中毒狀態已解除。\n";
        }
    }
}

void Character::applyStrength(int bonus, int turns) {
    if (strengthTurns_ == 0) { // 如果沒有強化，則施加強化效果
        std::cout << name_ << " 獲得了強化！\n";
    }
    else {
        std::cout << name_ << " 的強化狀態刷新了！\n";
    }
    strengthBonus_ = bonus;
    strengthTurns_ = turns;
}

void Character::decreaseStrengthTurns() {
    if (strengthTurns_ > 0) {
        strengthTurns_--;
        if (strengthTurns_ == 0) {
            strengthBonus_ = 0; // 效果結束，移除攻擊力加成
            std::cout << name_ << " 的強化狀態已解除。\n";
        }
    }
}