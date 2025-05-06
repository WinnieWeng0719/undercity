#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <string>

using namespace std;

template <typename T>
std::string to_string_custom(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

// 物品類別
class Item {
private:
    string name_;
    int value_;    // 物品效果值
    int level_;    // 物品等級

public:
    Item(string n, int v, int l = 1) : name_(n), value_(v), level_(l) {}

    virtual void display() {
        cout << name_ << " (等級: " << level_ << ", 效果: " << value_ << ")\n";
    }

    virtual bool isWeapon() const {
        return false;
    }

    // Getter 方法
    string getName() const { return name_; }
    int getValue() const { return value_; }
    int getLevel() const { return level_; }
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

    virtual void display() {
        cout << getName() << " (等級: " << getLevel() << ", 耐久度: " << durability_ << ")\n";
    }

    virtual bool isWeapon() const {
        return true;
    }

    // Getter 方法
    int getDurability() const { return durability_; }
};

// 藥水類別，繼承自 Item
class Potion : public Item {
public:
    Potion(string n, int v) : Item(n, v) {}

    virtual void display() {
        cout << getName() << " (等級: " << getLevel() << ", 效果: " << getValue() << ")\n";
    }
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

public:
    // 初始化列表中沒有使用 nullptr，改為 NULL
    Character(string n, int h, int a, int d)
        : name_(n), health_(h), attack_(a), defense_(d), experience_(0), equippedWeapon_(NULL) {}

    void takeDamage(int damage) {
        health_ -= max(0, damage - defense_);
    }

    void heal(int amount) {
        health_ += amount;
    }

    bool isAlive() {
        return health_ > 0;
    }

    void addItem(Item* item) {
        inventory_.push_back(item);
    }

    void displayStats() {
        cout << name_ << " - 目前血量: " << health_ << " 攻擊力: " << attack_ << " 防禦力: " << defense_ << " 等級: " << level_ << endl;
    }

    int getAttack() const {
        return attack_ + (equippedWeapon_ ? equippedWeapon_->getValue() : 0);
    }

    void displayInventory() const {
        cout <<"\n#-------------#" ;
        cout << "\n|     背包     |"<<endl;
        cout << "#-------------#"<<endl<<endl;
        if (inventory_.empty()) {
            cout << "背包是空的！\n";
        } else {
            for (int i = 0; i < inventory_.size(); ++i) {
                cout << i + 1 << ". ";
                inventory_[i]->display();
            }
        }
        cout << inventory_.size() + 1 << ". 退出背包\n"<<endl;    // 加入退出選項
    }

    void useItem(int index) {
        if (index >= 0 && index < inventory_.size()) {
            Item* item = inventory_[index];
            if (item->getName() == "治療藥水") {
                heal(item->getValue());
                cout << name_ << " 使用了治療藥水，恢復了 " << item->getValue() << " 生命！\n";
                inventory_.erase(inventory_.begin() + index);    // 使用藥水後從背包刪除
                delete item;    // 釋放記憶體
            } else if (item->isWeapon()) {
                Weapon* weapon = dynamic_cast<Weapon*>(item);
                if (weapon) {
                    if (!weapon->isBroken()) {
                        if (equippedWeapon_ != NULL) {
                            cout << "你已經裝備了 " << equippedWeapon_->getName() << "，無法裝備更多武器。\n";
                        } else {
                            equippedWeapon_ = weapon;    // 將選中的武器設為裝備
                            cout << name_ << " 裝備了 " << weapon->getName() << "，攻擊力提升了 " << weapon->getValue() << "！\n";
                            inventory_.erase(inventory_.begin() + index);    // 裝備後從背包移除
                            // 注意：這裡 weapon 指標現在是 equippedWeapon_，不要在這裡 delete
                        }
                    } else {
                        cout << weapon->getName() << " 已經損壞，無法使用。\n";
                    }
                }
            }
        } else {
            cout << "無效的物品。"<<endl;
            cout <<endl ;
        }
    }

    void displayEquippedWeapon() const {
        if (equippedWeapon_ != NULL) {
            cout << "目前裝備的武器: " << equippedWeapon_->getName()
                 << " (等級: " << equippedWeapon_->getLevel() << ", "
                 << "耐久度: " << equippedWeapon_->getDurability() << ")\n";
        } else {
            cout << "目前沒有裝備武器。\n";
        }
    }

    // 新增升級功能
    void gainExperience(int exp) {
        experience_ += exp;
        cout << "獲得了 " << exp << " 點經驗值！"<<endl<<endl;
        int experienceToNextLevel = level_ * 10; // 根據當前等級，升級所需的經驗值是等級 * 10
        if (experience_ >= experienceToNextLevel) {    // 升級條件
            experience_ -= experienceToNextLevel;    // 經驗值重置
            levelUp();
        }
    }

    void levelUp() {
        level_++;
        attack_ += 3;    // 升級時增加攻擊力
        health_ += 5;    // 升級時增加血量
        defense_ += 2;    // 升級時增加防禦力
        cout << name_ << " 升級了！現在等級: " << level_ <<endl;
        cout<< "攻擊力: " << attack_ <<" 生命值: " << health_ <<" 防禦力: " << defense_ << endl<<endl;
    }

    // Getter 方法
    string getName() const { return name_; }
    int getHealth() const { return health_; }
    int getAttackValue() const { return attack_; }
    int getDefense() const { return defense_; }
    int getLevelValue() const { return level_; }
    Weapon* getEquippedWeapon() const { return equippedWeapon_; }
    const vector<Item*>& getInventory() const { return inventory_; } // 新增獲取 inventory 的 const 引用
    void unequipWeapon() { equippedWeapon_ = NULL; }
    void removeItemFromInventory(int index) {
        if (index >= 0 && index < inventory_.size()) {
            delete inventory_[index]; // 釋放記憶體
            inventory_.erase(inventory_.begin() + index);
        }
    }
};

// 隨機生成敵人
Character generateEnemy(int playerLevel) {
    int enemyType = rand() % 100;
    int level = rand() % 3 + 1;
    if (playerLevel >= 12 && enemyType < 20) {
        return Character("宇宙無敵超級深淵惡魔-龍龍", 1000, rand() % 4 + 50, 20);    // 攻擊範圍 50 到 53
    }
    else if (playerLevel >= 9 &&enemyType < 30) {
        return Character("你的作業", 500, rand() % 400 + 40, 10);    // 攻擊範圍 40 到 43
    }
    else if (playerLevel >= 6 &&enemyType < 40) {
        return Character("林克", 250, rand() % 250 + 30, 15);    // 攻擊範圍 30 到 33
    }
    else if (playerLevel >= 3 &&enemyType < 50) {
        return Character("芙莉蓮花", 100, rand() % 100 + 15, 5);    // 攻擊範圍 15 到 18
    }
    else if (enemyType < 60) {
        return Character("哥布林士兵", 30, rand() % 4 + 10, 3);    // 哥布林士兵
    }
    else {
        int health = (level == 1) ? 12 : (level == 2) ? 15 : 17;
        return Character("哥布林 (等級 " + to_string_custom(level) + ")", health, rand() % 3 + 8, 1);
    }
}

// 戰鬥系統
void battle(Character& player, Character& enemy) {
    cout << "一隻 " << enemy.getName() << " 出現了！\n"<<endl;

    while (player.isAlive() && enemy.isAlive()) {
        // 顯示雙方血量
        cout << player.getName() << " - 目前血量: " << player.getHealth() << "  |  "
             << enemy.getName() << " - 目前血量: " << enemy.getHealth() << endl;

        // 顯示攻擊選項
        cout << "\n";
        cout << "╔═════════════════╗"<<endl;
        cout << "║     選擇攻擊     ║"<<endl;
        cout << "╚═════════════════╝"<<endl;
        cout << "1. 普通攻擊\n";
        cout << "2. 強力攻擊\n";
        cout << "3. 使用武器攻擊";
        if (player.getEquippedWeapon() != NULL) {
            cout << " (" << player.getEquippedWeapon()->getName() << ")\n";
            cout << "4. 使用物品\n";
        } else {
            cout << "\n4. 使用物品\n";
        }
        cout << "選擇動作: ";
        int action;
        cin >> action;

        int damage = 0;
        if (action == 1) {
            damage = player.getAttackValue();
            cout << player.getName() << " 發起了普通攻擊，造成了 " << damage << " 點傷害。\n";
        } else if (action == 2) {
            damage = player.getAttackValue() * 2;
            cout << player.getName() << " 發起了強力攻擊，造成了 " << damage << " 點傷害。\n";
        } else if (action == 3) {
            // 使用武器攻擊
            if (player.getEquippedWeapon() != NULL) {
                player.getEquippedWeapon()->use(); // 減少武器耐久度
                damage = player.getAttack();
                cout << player.getName() << " 使用 " << player.getEquippedWeapon()->getName() << " 攻擊，造成了 " << damage << " 點傷害。\n";
                if (player.getEquippedWeapon()->isBroken()) {
                    cout << player.getEquippedWeapon()->getName() << " 損壞了！\n";
                    player.unequipWeapon(); // 使用 unequipWeapon() 方法
                }
            } else {
                cout << "你沒有裝備任何武器，無法使用武器攻擊。\n";
                continue; // 返回戰鬥選單
            }
        } else if (action == 4) {
            // 使用物品
            player.displayInventory(); // 這個函式內部已經正確使用了 inventory_
            cout << "選擇一個物品來使用: ";
            int itemChoice;
            cin >> itemChoice;
            if (itemChoice > 0 && itemChoice <= player.getInventory().size()) { // 使用 getInventory().size()
                player.useItem(itemChoice - 1); // useItem 內部會正確操作 inventory_
            } else if (itemChoice == player.getInventory().size() + 1) { // 使用 getInventory().size()
                cout << "退出背包。\n";
                continue; // 返回戰鬥選單
            } else {
                cout << "無效的選項。\n";
                continue; // 返回戰鬥選單
            }
            continue; // 使用物品後跳過本回合的攻擊
        } else {
            cout << "無效的動作。\n";
            continue; // 返回戰鬥選單
        }
        enemy.takeDamage(damage);

        if (!enemy.isAlive()) {
            cout << enemy.getName() << " 被擊敗了！\n"<<endl;
            // 當擊敗的是哥布林士兵，掉落劍
            if (enemy.getName() == "哥布林士兵") {
                player.addItem(new Weapon("劍", 15, 3, 2));
                cout << "你撿到了 劍！\n";
            } else {
                // 若是其他敵人，掉落藥水
                player.addItem(new Potion("治療藥水", 20));
                cout << "你撿到了 治療藥水！\n";
            }
            player.gainExperience(4 + player.getLevelValue());
            break;
        }

        // 敵人回擊
        damage = enemy.getAttackValue();
        player.takeDamage(damage);
        if(enemy.getHealth() != 0)
        cout << enemy.getName() << " 攻擊了你，造成了 " << damage << " 點傷害。"<<endl<<endl;
        else
        {break;}
    }

    if (!player.isAlive()) {
        cout << "GAME OVER。"<<endl;
    }
}

// 主選單
void showMenu(bool& firstTime) {
    if (firstTime) {
        cout << "╔═══════════════════════╗"<<endl;
        cout << "║         主選單         ║"<<endl;
        cout << "╚═══════════════════════╝"<<endl;
    }
    cout << "1. 探索地下城\n2. 查看背包\n3. 查看裝備的武器\n4. 退出遊戲\n選擇一個選項: ";
}

int main() {
    srand(time(0));

    Character player("玩家", 21, 10, 3);
    bool firstTime = true;
    cout<<"歡迎來到地下城"<<endl;
    cout<< endl<<"等級: "<<player.getLevelValue()<<endl<<"攻擊力: " <<player.getAttackValue() <<" 生命值: " <<player.getHealth()<<" 防禦力: " <<player.getDefense()<<endl;
    while (true) {
        showMenu(firstTime);
        int choice;
        cin >> choice;


        if (choice == 1) {
            cout <<"#-------------#";
            cout << "\n已進入地下城..."<<endl;
            Character enemy = generateEnemy(player.getLevelValue());
            if (rand() % 3 == 0) {
                cout << "\n遇到敵人: " << enemy.getName() << endl;
                battle(player, enemy);
            } else {
                cout << "地下城一片寧靜... 什麼也沒有找到。\n"<<endl;;
            }
        } else if (choice == 2) {
            while (true) {
                player.displayInventory(); // 內部已正確使用 inventory_
                cout << "選擇一個選項: ";
                int itemChoice;
                cin >> itemChoice;
                if (itemChoice == player.getInventory().size() + 1) { // 使用 getInventory().size()
                    break;
                } else {
                    player.useItem(itemChoice - 1); // 內部已正確操作 inventory_
                }
            }
        } else if (choice == 3) {
            player.displayEquippedWeapon(); // 內部已正確使用 equippedWeapon_
        } else if (choice == 4) {
            cout << "退出遊戲。期待再次挑戰地下城!\n";
            break;
        } else {
            cout << "無效的選項，請再試一次。\n";
        }

        if (!player.isAlive()) {
            break;
        }
    }

    return 0;
}