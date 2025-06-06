#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm> // for std::max

using namespace std;

// 自訂的 to_string 函式，用於將數字轉換為字串
//template <typename T>
//std::string to_string_custom(const T& value) {
//    std::stringstream ss;
//    ss << value;
//    return ss.str();
//}

// 物品類別
class Item {
private:
    string name_;
    int value_;    // 物品效果值
    int level_;    // 物品等級

public:
    Item(string n, int v, int l = 1) : name_(n), value_(v), level_(l) {}

    virtual void display() const { // 加上 const
        cout << name_ << " (等級: " << level_ << ", 效果: " << value_ << ")\n";
    }

    virtual bool isWeapon() const {
        return false;
    }

    // 新增虛擬函式來應用物品效果，需要傳入目標角色和攻擊者
    // 預設不做任何事，由子類別覆寫
    virtual void applyEffect(class Character& target, class Character& attacker) {
        // 預設無效果
    }

    // Getter 方法
    string getName() const { return name_; }
    int getValue() const { return value_; }
    int getLevel() const { return level_; }

    // 虛擬析構函式，確保正確釋放記憶體
    virtual ~Item() {}
};

// 武器類別，繼承自 Item
class Weapon : public Item {
private:
    int durability_;    // 武器的耐久度

public:
    Weapon(string n, int v, int d, int l = 1) : Item(n, v, l), durability_(d) {}

    void use() {
        if (durability_ > 0) {
            durability_--;
        }
    }

    bool isBroken() const {
        return durability_ == 0;
    }

    virtual void display() const override { // 加上 const
        cout << getName() << " (等級: " << getLevel() << ", 耐久度: " << durability_ << ", 攻擊: " << getValue() << ")\n";
    }

    virtual bool isWeapon() const override {
        return true;
    }

    // Getter 方法
    int getDurability() const { return durability_; }
};

// 新增弓類別，繼承自 Weapon
class Bow : public Weapon {
public:
    Bow(string n, int v, int d, int l = 1) : Weapon(n, v, d, l) {}

    virtual void display() const override {
        cout << getName() << " (等級: " << getLevel() << ", 耐久度: " << getDurability() << ", 攻擊: " << getValue() << ", 類型: 弓)\n";
    }
};

// 新增斧頭類別，繼承自 Weapon
class Axe : public Weapon {
public:
    Axe(string n, int v, int d, int l = 1) : Weapon(n, v, d, l) {}

    virtual void display() const override {
        cout << getName() << " (等級: " << getLevel() << ", 耐久度: " << getDurability() << ", 攻擊: " << getValue() << ", 類型: 斧)\n";
    }
};

// 藥水類別，繼承自 Item
class Potion : public Item {
public:
    Potion(string n, int v) : Item(n, v) {}

    virtual void display() const override { // 加上 const
        cout << getName() << " (等級: " << getLevel() << ", 效果: " << getValue() << ")\n";
    }

    // 所有藥水都有 applyEffect，具體效果在子類別實現
    virtual void applyEffect(class Character& target, class Character& attacker) override = 0; // 設為純虛擬函式
};

// 治療藥水類別
class HealthPotion : public Potion {
public:
    HealthPotion(string n, int v) : Potion(n, v) {}

    virtual void applyEffect(class Character& target, class Character& attacker) override;
};

// 新增毒藥水類別
class PoisonPotion : public Potion {
public:
    PoisonPotion(string n, int v) : Potion(n, v) {} // value_在這裡表示毒素持續回合數

    virtual void applyEffect(class Character& target, class Character& attacker) override;
};

// 新增強化藥水類別
class StrengthPotion : public Potion {
public:
    StrengthPotion(string n, int v) : Potion(n, v) {} // value_在這裡表示攻擊力增益值

    virtual void applyEffect(class Character& target, class Character& attacker) override;
};

// 新增爆炸藥水類別
class ExplosionPotion : public Potion {
public:
    ExplosionPotion(string n, int v) : Potion(n, v) {} // value_在這裡表示傷害值

    virtual void applyEffect(class Character& target, class Character& attacker) override;
};

// 角色類別
class Character {
private:
    string name_;
    int health_;
    int attack_;
    int defense_;
    int experience_;    // 新增經驗值屬性
    int level_ = 1;
    vector<Item*> inventory_;
    Weapon* equippedWeapon_;    // 裝備的武器

    // 新增狀態變數
    int poisonTurns_;   // 中毒剩餘回合數
    int strengthTurns_; // 強化剩餘回合數
    int strengthBonus_; // 強化提供的攻擊力加成

public:
    Character(string n, int h, int a, int d)
        : name_(n), health_(h), attack_(a), defense_(d), experience_(0), equippedWeapon_(nullptr),
          poisonTurns_(0), strengthTurns_(0), strengthBonus_(0) {} // 初始化新的狀態變數

    // 析構函式，釋放背包中物品的記憶體
    ~Character() {
        for (Item* item : inventory_) {
            delete item;
        }
        inventory_.clear(); // 清空向量
        // equippedWeapon_ 指向的記憶體是由 addItem 時加到 inventory_ 中的物件，所以不需要單獨 delete
        // 在 removeItemFromInventory 或 unequipWeapon(true) 時已經 delete 過了
        equippedWeapon_ = nullptr; // 確保指標為空
    }

    void takeDamage(int damage) {
        health_ -= max(0, damage - defense_);
    }

    void heal(int amount) {
        health_ += amount;
    }

    bool isAlive() const {
        return health_ > 0;
    }

    void addItem(Item* item) {
        inventory_.push_back(item);
    }

    void displayStats() const {
        cout << name_ << " - 目前血量: " << health_<<" 攻擊："<< attack_ ;
        if (strengthTurns_ > 0) {
            cout << " (強化中，剩餘" << strengthTurns_ << "回合)";
        }
        if (poisonTurns_ > 0) {
            cout << " (中毒中，剩餘" << poisonTurns_ << "回合)";
        }
        cout << endl;
    }

    int getAttack() const {
        return attack_ + (equippedWeapon_ ? equippedWeapon_->getValue() : 0) + strengthBonus_;
    }

    void displayInventory() const {
        cout << "\n#-------------#";
        cout << "\n|     背包     |" << endl;
        cout << "#-------------#" << endl << endl;
        if (inventory_.empty()) {
            cout << "背包是空的！\n";
        }
        else {
            for (int i = 0; i < inventory_.size(); ++i) {
                cout << i + 1 << ". ";
                inventory_[i]->display();
            }
        }
        cout << inventory_.size() + 1 << ". 退出背包\n" << endl;
    }

    void useItem(int index, Character& targetCharacter) { // useItem 函式需要知道目標角色
        if (index >= 0 && index < inventory_.size()) {
            Item* item = inventory_[index];
            if (item->isWeapon()) {
                Weapon* weapon = dynamic_cast<Weapon*>(item);
                if (weapon) {
                    if (!weapon->isBroken()) {
                        if (equippedWeapon_ != nullptr) {
                            cout << "你已經裝備了 " << equippedWeapon_->getName() << "，請先卸下才能裝備其他武器。\n";
                        }
                        else {
                            equippedWeapon_ = weapon;
                            cout << name_ << " 裝備了 " << weapon->getName() << "，攻擊力提升了 " << weapon->getValue() << "！\n";
                            inventory_.erase(inventory_.begin() + index);
                        }
                    }
                    else {
                        cout << weapon->getName() << " 已經損壞，無法使用。\n";
                    }
                }
            } else { // 如果是藥水，則應用效果
                Potion* potion = dynamic_cast<Potion*>(item);
                if (potion) {
                    potion->applyEffect(targetCharacter, *this); // 傳入目標角色和玩家自己
                    inventory_.erase(inventory_.begin() + index);
                    delete item; // 釋放藥水記憶體
                }
            }
        }
        else {
            cout << "無效的物品。\n";
            cout << endl;
        }
    }

    void displayEquippedWeapon() const {
        if (equippedWeapon_ != nullptr) {
            equippedWeapon_->display();
        }
        else {
            cout << "目前沒有裝備武器。\n";
        }
    }

    void gainExperience(int exp) {
        experience_ += exp;
        cout << "獲得了 " << exp << " 點經驗值！" << endl << endl;
        int experienceToNextLevel = level_ * 10;
        if (experience_ >= experienceToNextLevel) {
            experience_ -= experienceToNextLevel;
            levelUp();
        }
    }

    void levelUp() {
        level_++;
        attack_ += 3;
        health_ += 5;
        defense_ += 2;
        cout << name_ << " 升級了！現在等級: " << level_ << endl;
        cout << "攻擊力: " << attack_ << " 生命值: " << health_ << " 防禦力: " << defense_ << endl << endl;
    }

    string getName() const { return name_; }
    int getHealth() const { return health_; }
    int getAttackValue() const { return attack_; }
    int getDefense() const { return defense_; }
    int getLevelValue() const { return level_; }
    Weapon* getEquippedWeapon() const { return equippedWeapon_; }
    const vector<Item*>& getInventory() const { return inventory_; }

    void unequipWeapon(bool silent = false) {
        if (equippedWeapon_ != nullptr) {
            if (!silent) {
                inventory_.push_back(equippedWeapon_);
                cout << equippedWeapon_->getName() << " 已從裝備欄卸下並放回背包。\n";
            } else {
                delete equippedWeapon_;
            }
            equippedWeapon_ = nullptr;
        } else {
            if (!silent) {
                cout << "沒有裝備任何武器。\n";
            }
        }
    }

    void removeItemFromInventory(int index) {
        if (index >= 0 && index < inventory_.size()) {
            if (inventory_[index] == equippedWeapon_) {
                equippedWeapon_ = nullptr;
            }
            delete inventory_[index];
            inventory_.erase(inventory_.begin() + index);
        }
    }

    // 新增毒素相關方法
    void applyPoison(int turns) {
        if (poisonTurns_ == 0) { // 如果沒有中毒，則施加中毒效果
            cout << name_ << " 中毒了！\n";
        }
        poisonTurns_ = turns;
    }

    void takePoisonDamage() {
        if (poisonTurns_ > 0) {
            int poisonDmg = 5; // 每回合毒素傷害
            health_ -= poisonDmg;
            cout << name_ << " 因中毒損失了 " << poisonDmg << " 點生命！\n";
            poisonTurns_--;
            if (poisonTurns_ == 0) {
                cout << name_ << " 的毒素效果已解除。\n";
            }
        }
    }

    // 新增強化藥水相關方法
    void applyStrength(int bonus, int turns) {
        if (strengthTurns_ == 0) { // 如果沒有強化，則施加強化效果
            cout << name_ << " 獲得了強化！\n";
        } else {
            cout << name_ << " 的強化效果刷新了！\n";
        }
        strengthBonus_ = bonus;
        strengthTurns_ = turns;
    }

    void decreaseStrengthTurns() {
        if (strengthTurns_ > 0) {
            strengthTurns_--;
            if (strengthTurns_ == 0) {
                strengthBonus_ = 0; // 效果結束，移除攻擊力加成
                cout << name_ << " 的強化效果已解除。\n";
            }
        }
    }
};

// 治療藥水效果實現
void HealthPotion::applyEffect(Character& target, Character& attacker) {
    target.heal(getValue());
    cout << target.getName() << " 使用了 " << getName() << "，恢復了 " << getValue() << " 生命！\n";
}

// 毒藥水效果實現
void PoisonPotion::applyEffect(Character& target, Character& attacker) {
    // 毒藥水只能對敵人使用
    if (&target != &attacker) { // 確保目標不是使用者本身 (即敵人)
        target.applyPoison(getValue()); // value_ 是回合數
        cout << attacker.getName() << " 對 " << target.getName() << " 使用了 " << getName() << "，使其中毒 " << getValue() << " 回合！\n";
    } else {
        cout << attacker.getName() << " 無法對自己使用毒藥水。\n";
    }
}

// 強化藥水效果實現
void StrengthPotion::applyEffect(Character& target, Character& attacker) {
    // 強化藥水只能對自己使用
    if (&target == &attacker) {
        target.applyStrength(getValue(), 3); // value_ 是攻擊力加成，固定持續 3 回合
        cout << target.getName() << " 使用了 " << getName() << "，攻擊力提升 " << getValue() << " 點，持續 3 回合！\n";
    } else {
        cout << attacker.getName() << " 無法對敵人使用強化藥水。\n";
    }
}

// 爆炸藥水效果實現
void ExplosionPotion::applyEffect(Character& target, Character& attacker) {
    // 爆炸藥水對敵人造成傷害
    if (&target != &attacker) {
        int explosionDamage = getValue(); // value_ 是傷害值
        target.takeDamage(explosionDamage);
        cout << attacker.getName() << " 對 " << target.getName() << " 使用了 " << getName() << "，造成了 " << explosionDamage << " 點傷害！\n";
    } else {
        cout << attacker.getName() << " 無法對自己使用爆炸藥水。\n";
    }
}

// 隨機生成敵人
Character generateEnemy(int playerLevel) {
    int enemyType = rand() % 100;
    int level = rand() % 3 + 1;
    if (playerLevel >= 12 && enemyType < 20) {
        return Character("宇宙無敵超級深淵惡魔-龍龍", 1000, rand() % 4 + 50, 20);    // 攻擊範圍 50 到 53
    }
    else if (playerLevel >= 9 && enemyType < 30) {
        return Character("你的作業", 500, rand() % 400 + 40, 10);    // 攻擊範圍 40 到 43
    }
    else if (playerLevel >= 6 && enemyType < 40) {
        return Character("林克", 250, rand() % 250 + 30, 15);    // 攻擊範圍 30 到 33
    }
    else if (playerLevel >= 3 && enemyType < 50) {
        return Character("芙莉蓮花", 100, rand() % 100 + 15, 5);    // 攻擊範圍 15 到 18
    }
    else if(enemyType < 60){
    int health = (level == 1) ? 12 : (level == 2) ? 15 : 17;
    return Character("哥布林 (等級 " + to_string(level) + ")", health, rand() % 3 + 8, 1);
    }
    else if (enemyType < 70) {
        return Character("哥布林士兵", 30, rand() % 4 + 10, 3);    // 哥布林士兵
    }
    else if (enemyType < 80){ // 哥布林弓箭手
        return Character("哥布林弓箭手", 25, rand() % 5 + 12, 2);
    }
    else if(enemyType < 90){ // 哥布林狂戰士
        return Character("哥布林狂戰士", 40, rand() % 6 + 18, 4);
    }
    else {
        int health = (level == 1) ? 12 : (level == 2) ? 15 : 17;
        return Character("哥布林 (等級 " + to_string(level) + ")", health , rand() % 3 + 8, 1);
    }
    
}

// 新增處理物品掉落的函式
void handleItemDrop(Character& player, const string& itemName, Item* droppedItem, const string& icon) {
    char choice;
    bool validInput = false;
    while (!validInput) {
        cout << icon << " 敵人掉落了" << itemName << "，要撿起來嗎？（y/n）：";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            player.addItem(droppedItem);
            cout << "你撿到了 " << itemName << "！\n";
            validInput = true;
        } else if (choice == 'n' || choice == 'N') {
            cout << "你選擇不撿起" << itemName << "。\n";
            delete droppedItem; // 如果不撿取，就刪除這個物品
            validInput = true;
        } else {
            cout << "無效選擇，請重新輸入。\n";
        }
    }
}

// 戰鬥系統
void battle(Character& player, Character& enemy) {
    cout << "一隻 " << enemy.getName() << " 出現了！\n" << endl;

    while (player.isAlive() && enemy.isAlive()) {

        player.decreaseStrengthTurns(); // 減少玩家強化回合數
        enemy.takePoisonDamage();       // 敵人中毒扣血

        // 顯示雙方血量和狀態
        player.displayStats();
        enemy.displayStats();

        // 如果敵人因毒素死亡，則直接結束戰鬥
        if (!enemy.isAlive()) {
            cout << enemy.getName() << " 因中毒而死亡！\n" << endl;
            player.gainExperience(4 + player.getLevelValue());
            break;
        }
        
        // 新增敵人血量高於玩家時的警告
        if (enemy.getHealth() > player.getHealth()) {
            cout << "⚠️ 警告：敵人血量高於你，請小心應戰！\n";
        }
        
        // 顯示攻擊選項
        cout << "\n";
        cout << "╔═════════════════╗" << endl;
        cout << "║     選擇攻擊    ║" << endl;
        cout << "╚═════════════════╝" << endl;
        cout << "1. 普通攻擊\n";
        cout << "2. 強力攻擊\n";
        cout << "3. 使用武器攻擊";
        if (player.getEquippedWeapon() != nullptr) {
            cout << " (" << player.getEquippedWeapon()->getName() << ")\n";
        }
        else {
            cout << " (未裝備武器)\n";
        }
        cout << "4. 使用物品\n";
        cout << "選擇動作: ";
        int action;
        cin >> action;

        int damage = 0;
        if (action == 1) {
            damage = player.getAttack(); // 使用 getAttack() 包含武器和強化效果
            cout << player.getName() << " 發起了普通攻擊，造成了 " << damage << " 點傷害。\n";
            cout << enemy.getName() << " 的防禦力為他抵擋了 " << enemy.getDefense() << " 點傷害。\n";
            cout << enemy.getName() << " 總共受到 " << max(0, damage - enemy.getDefense()) << " 點傷害。\n"<<endl;
            enemy.takeDamage(damage);
        }
        else if (action == 2) {
            damage = player.getAttack() * 2; // 使用 getAttack() 包含武器和強化效果
            cout << player.getName() << " 發起了強力攻擊，造成了 " << damage << " 點傷害。\n";
            cout << enemy.getName() << " 的防禦力為他抵擋了 " << enemy.getDefense() << " 點傷害。\n";
            cout << enemy.getName() << " 總共受到 " << max(0, damage - enemy.getDefense()) << " 點傷害。\n"<<endl;
            enemy.takeDamage(damage);
        }
        else if (action == 3) {
            if (player.getEquippedWeapon() != nullptr) {
                player.getEquippedWeapon()->use();
                damage = player.getAttack(); // 使用 getAttack() 包含武器和強化效果
                cout << player.getName() << " 使用 " << player.getEquippedWeapon()->getName() << " 攻擊，造成了 " << damage << " 點傷害。\n";
                cout << enemy.getName() << " 的防禦力為他抵擋了 " << enemy.getDefense() << " 點傷害。\n";
                cout << enemy.getName() << " 總共受到 " << max(0, damage - enemy.getDefense()) << " 點傷害。\n"<<endl;
                enemy.takeDamage(damage);
                if (player.getEquippedWeapon()->isBroken()) {
                    cout << player.getEquippedWeapon()->getName() << " 損壞了！\n";
                    player.unequipWeapon(true);
                }
            }
            else {
                cout << "你沒有裝備任何武器，無法使用武器攻擊。\n";
                continue;
            }
        }
        else if (action == 4) {
            if (player.getInventory().empty()){
                cout << "背包是空的，沒有物品可以使用。\n";
                continue;
            }
            player.displayInventory();
            cout << "選擇一個物品來使用: ";
            int itemChoice;
            cin >> itemChoice;
            if (itemChoice > 0 && itemChoice <= player.getInventory().size()) {
                // 根據物品類型，決定對誰使用
                Item* selectedItem = player.getInventory()[itemChoice - 1];
                if (dynamic_cast<PoisonPotion*>(selectedItem) || dynamic_cast<ExplosionPotion*>(selectedItem)) {
                    // 毒藥水和爆炸藥水用於敵人
                    player.useItem(itemChoice - 1, enemy);
                } else if (dynamic_cast<HealthPotion*>(selectedItem) || dynamic_cast<StrengthPotion*>(selectedItem)) {
                    // 治療藥水和強化藥水用於自己
                    player.useItem(itemChoice - 1, player);
                } else {
                    // 其他物品（如裝備）的預設處理
                    player.useItem(itemChoice - 1, player); // 對自己使用 (武器裝備)
                }
            }
            else if (itemChoice == player.getInventory().size() + 1) {
                cout << "退出背包。\n";
                continue;
            }
            else {
                cout << "無效的選項。\n";
                continue;
            }
            continue; // 使用物品後跳過本回合的攻擊
        }
        else {
            cout << "無效的動作。\n";
            continue;
        }

        if (!enemy.isAlive()) {
            cout << enemy.getName() << " 被擊敗了！\n" << endl;
            // 根據敵人掉落不同的物品
            string enemyName = enemy.getName();

            // 判斷是否為哥布林類型的敵人
            bool isGoblinEnemy = (enemyName == "哥布林士兵" || enemyName == "哥布林弓箭手" || enemyName == "哥布林狂戰士" || enemyName.rfind("哥布林 (等級", 0) == 0);

            if (enemyName == "哥布林士兵") {
                handleItemDrop(player, "劍", new Weapon("劍", 15, 3, 2), "🗡️");
            } else if (enemyName == "哥布林弓箭手") {
                handleItemDrop(player, "短弓", new Bow("短弓", 12, 5, 2), "🏹");
            } else if (enemyName == "哥布林狂戰士") {
                handleItemDrop(player, "石斧", new Axe("石斧", 18, 4, 3), "🪓");
            }
            // 哥布林敵人有機會掉落藥水
            else if (isGoblinEnemy) {
                int potionDropChance = rand() % 100;
                if (potionDropChance < 30) { // 30% 機率掉落治療藥水
                    handleItemDrop(player, "治療藥水", new HealthPotion("治療藥水", 20), "🧪");
                } else if (potionDropChance < 50) { // 20% 機率掉落毒藥水 (累積機率 30+20=50%)
                    handleItemDrop(player, "毒藥水", new PoisonPotion("毒藥水", 3), "☠️"); // 持續 3 回合
                } else if (potionDropChance < 70) { // 20% 機率掉落強化藥水 (累積機率 50+20=70%)
                    handleItemDrop(player, "強化藥水", new StrengthPotion("強化藥水", 5), "💪"); // 攻擊力加成 5
                } else if (potionDropChance < 90) { // 20% 機率掉落爆炸藥水 (累積機率 70+20=90%)
                    handleItemDrop(player, "爆炸藥水", new ExplosionPotion("爆炸藥水", 20), "💥"); // 造成 20 點傷害
                } else { // 10% 機率什麼都沒掉落 (累積機率 90+10=100%)
                    cout << "敵人什麼也沒掉落。\n";
                }
            } else { // 其他非哥布林敵人，預設只掉落治療藥水
                handleItemDrop(player, "治療藥水", new HealthPotion("治療藥水", 20), "🧪");
            }
            player.gainExperience(4 + player.getLevelValue());
            break;
        }

        // 敵人回擊
        damage = enemy.getAttackValue();
        player.takeDamage(damage);
        
            cout << enemy.getName() << " 攻擊了你，造成了 " << damage << " 點傷害。" << endl ;
            cout << player.getName() << " 的防禦力為他抵擋了 " << player.getDefense() << " 點傷害。\n";
            cout << player.getName() << " 總共受到 " << max(0, damage - player.getDefense()) << " 點傷害。\n"<<endl;
        
    }

    if (!player.isAlive()) {
        cout << "GAME OVER。" << endl;
    }
}

// 主選單
void showMenu() {
    cout << "╔═══════════════════════╗" << endl;
    cout << "║         主選單        ║" << endl;
    cout << "╚═══════════════════════╝" << endl;
    cout << "1. 探索地下城\n2. 查看背包\n3. 查看裝備的武器\n4. 卸下裝備\n5. 退出遊戲\n選擇一個選項: ";
}

int main() {
    srand(time(0));

    Character player("玩家", 21, 10, 3);
    cout << "歡迎來到地下城" << endl;
    cout << endl << "等級: " << player.getLevelValue() << endl << "攻擊力: " << player.getAttackValue() << " 生命值: " << player.getHealth() << " 防禦力: " << player.getDefense() << endl;

    int consecutiveNothingFound = 0;

    while (true) {
        showMenu();
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "#-------------#";
            cout << "\n已進入地下城..." << endl;
            if (consecutiveNothingFound < 2) {
                if (rand() % 3 == 0) { // 33% 機率遇到敵人
                    Character enemy = generateEnemy(player.getLevelValue());
                    cout << "\n遇到敵人: " << enemy.getName() << endl;
                    battle(player, enemy);
                    consecutiveNothingFound = 0;
                }
                else {
                    cout << "地下城一片寧靜... 什麼也沒有找到。\n" << endl;;
                    consecutiveNothingFound++;
                }
            }
            else {
                // 連續兩次什麼都沒找到，這次強制遇到敵人
                Character enemy = generateEnemy(player.getLevelValue());
                cout << "\n遇到敵人: " << enemy.getName() << endl;
                battle(player, enemy);
                consecutiveNothingFound = 0;
            }
        }
        else if (choice == 2) {
            while (true) {
                if (player.getInventory().empty()){
                    cout << "\n背包是空的！\n" << endl;
                    break; // 如果背包空了，就直接退出循環
                }
                player.displayInventory();
                cout << "選擇一個選項: ";
                int itemChoice;
                cin >> itemChoice;
                if (itemChoice == player.getInventory().size() + 1) {
                    break;
                }
                else {
                    // 使用物品時，需要判斷目標
                    Item* selectedItem = player.getInventory()[itemChoice - 1];
                    if (dynamic_cast<PoisonPotion*>(selectedItem) || dynamic_cast<ExplosionPotion*>(selectedItem)) {
                         cout << "請在戰鬥中使用此類藥水！\n"; // 這些藥水只能在戰鬥中對敵人使用
                    } else {
                        player.useItem(itemChoice - 1, player); // 對自己使用
                    }
                }
            }
        }
        else if (choice == 3) {
            player.displayEquippedWeapon();
        }
        else if (choice == 4) {
            player.unequipWeapon();
        }
        else if (choice == 5) {
            cout << "退出遊戲。期待再次挑戰地下城!\n";
            break;
        }
        else {
            cout << "無效的選項，請再試一次。\n";
        }

        if (!player.isAlive()) {
            break;
        }
    }

    return 0;
}