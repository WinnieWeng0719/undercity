#include "Character.h"
#include "Item.h"  
#include "Weapon.h"  // �ݭn Weapon ���O
#include "Potion.h"  // �ݭn Potion ���O
// �ݭn Item ���O������w�q
// �`�N�G�o�̤��ݭn�B�~�]�t Weapon.h �� Potion.h�A�]�� Item.h �w�g�]�t�F���̪��ŧi�A
// �B Character.cpp ���� Weapon �M Potion ���ާ@���O�z�L Item ���ЩM dynamic_cast �i�檺�C

Character::Character(std::string n, int h, int a, int d)
    : name_(n), health_(h), attack_(a), defense_(d), experience_(0), level_(1), equippedWeapon_(nullptr),
    poisonTurns_(0), strengthTurns_(0), strengthBonus_(0) {
}

Character::~Character() {
    for (Item* item : inventory_) {
        delete item;
    }
    inventory_.clear();
    equippedWeapon_ = nullptr; // �T�O���Ь���
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
    std::cout << name_ << " - �ثe��q: " << health_ << " �����G" << attack_;
    if (strengthTurns_ > 0) {
        std::cout << "���A :" << std::endl << " �j�Ƥ��A�Ѿl(" << strengthTurns_ << "�^�X)";
    }
    if (poisonTurns_ > 0) {
        std::cout << "���A :" << std::endl << " ���r���A�Ѿl(" << poisonTurns_ << "�^�X)";
    }
    std::cout << std::endl;
}

int Character::getAttack() const {
    return attack_ + (equippedWeapon_ ? (0.5 * equippedWeapon_->getValue()) : 0) + strengthBonus_;
}

void Character::displayItemList() const {
    std::cout << "\n#------------------#";
    std::cout << "\n|    ���~�M��      |" << std::endl;
    std::cout << "#------------------#" << std::endl << std::endl;
    if (inventory_.empty()) {
        std::cout << "���~�M��O�Ū��I\n";
    }
    else {
        for (int i = 0; i < inventory_.size(); ++i) {
            std::cout << i + 1 << ". ";
            inventory_[i]->display();
        }
    }
    std::cout << inventory_.size() + 1 << ". �h�X���~�M��\n" << std::endl;
}

void Character::useItem(int index, Character& targetCharacter) {
    if (index >= 0 && index < inventory_.size()) {
        Item* item = inventory_[index];
        if (item->isWeapon()) {
            Weapon* weapon = dynamic_cast<Weapon*>(item);
            if (weapon) {
                if (!weapon->isBroken()) {
                    if (equippedWeapon_ != nullptr) {
                        std::cout << "���a�w�˳ƤF " << equippedWeapon_->getName() << "�A�Х����U�~��˳ƨ�L�Z���C\n";
                    }
                    else {
                        equippedWeapon_ = weapon;
                        std::cout << name_ << " �˳ƤF " << weapon->getName() << "�A�����O���ɤF " << weapon->getValue() << "�I\n";
                        inventory_.erase(inventory_.begin() + index);
                    }
                }
                else {
                    std::cout << weapon->getName() << " �w�l���A�L�k�ϥΡC\n";
                }
            }
        }
        else { // �p�G�O�Ĥ��A�h���ήĪG
            Potion* potion = dynamic_cast<Potion*>(item);
            if (potion) {
                potion->applyEffect(targetCharacter, *this); // �ǤJ�ؼШ���M���a�ۤv
                inventory_.erase(inventory_.begin() + index);
                delete item; // �����Ĥ��O����
            }
        }
    }
    else {
        std::cout << "�L�Ī����~�C\n";
        std::cout << std::endl;
    }
}

void Character::displayEquippedWeapon() const {
    if (equippedWeapon_ != nullptr) {
        equippedWeapon_->display();
    }
    else {
        std::cout << "�ثe�L�˳ƪZ���C\n";
    }
}

void Character::gainExperience(int exp) {
    experience_ += exp;
    std::cout << "�W�[�F " << exp << " �I�g��ȡI" << std::endl << std::endl;
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
    std::cout << name_ << " ���ŤW�ɤF�I�ثe����: " << level_ << std::endl;
    std::cout << "�����O: " << attack_ << " �ͩR��: " << health_ << " ���m�O: " << defense_ << std::endl << std::endl;
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
            std::cout << equippedWeapon_->getName() << " �w���U�é�^�I�]�C\n";
        }
        else {
            delete equippedWeapon_;
        }
        equippedWeapon_ = nullptr;
    }
    else {
        if (!silent) {
            std::cout << "�L�˳ƪZ���C\n";
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
    if (poisonTurns_ == 0) { // �p�G�S�����r�A�h�I�[���r�ĪG
        std::cout << name_ << " ���r�F�I\n";
    }
    poisonTurns_ = turns;
}

void Character::takePoisonDamage() {
    if (poisonTurns_ > 0) {
        int poisonDmg = 5; // �C�^�X�r���ˮ`
        health_ -= poisonDmg;
        std::cout << name_ << " �]���r��֤F " << poisonDmg << " �I�ͩR�I\n";
        poisonTurns_--;
        if (poisonTurns_ == 0) {
            std::cout << name_ << " �����r���A�w�Ѱ��C\n";
        }
    }
}

void Character::applyStrength(int bonus, int turns) {
    if (strengthTurns_ == 0) { // �p�G�S���j�ơA�h�I�[�j�ƮĪG
        std::cout << name_ << " ��o�F�j�ơI\n";
    }
    else {
        std::cout << name_ << " ���j�ƪ��A��s�F�I\n";
    }
    strengthBonus_ = bonus;
    strengthTurns_ = turns;
}

void Character::decreaseStrengthTurns() {
    if (strengthTurns_ > 0) {
        strengthTurns_--;
        if (strengthTurns_ == 0) {
            strengthBonus_ = 0; // �ĪG�����A���������O�[��
            std::cout << name_ << " ���j�ƪ��A�w�Ѱ��C\n";
        }
    }
}