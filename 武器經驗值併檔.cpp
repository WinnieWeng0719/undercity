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
    public:
        string name;
        int value;  // 物品效果值
        int level;  // 物品等級
    
        Item(string n, int v, int l = 1) : name(n), value(v), level(l) {}
    
        virtual void display() {
            cout << name << " (等級: " << level << ", 效果: " << value << ")\n";
        }
    
        virtual bool isWeapon() const {
            return false;
        }
    };
    
    // 武器類別，繼承自 Item
    class Weapon : public Item {
    public:
        int durability;  // 武器的耐久度
    
        Weapon(string n, int v, int d, int l = 1) : Item(n, v, l), durability(d) {}
    
        void use() {
            if (durability > 0) {
                durability--;
            }
        }
    
        bool isBroken() const {
            return durability == 0;
        }
    
        virtual void display() {
            cout << name << " (等級: " << level << ", 耐久度: " << durability << ")\n";
        }
    
        virtual bool isWeapon() const {
            return true;
        }
    };
    
    // 藥水類別，繼承自 Item
    class Potion : public Item {
    public:
        Potion(string n, int v) : Item(n, v) {}
    
        virtual void display() {
            cout << name << " (等級: " << level << ", 效果: " << value << ")\n";
        }
    };

// 角色類別
class Character {
public:
    string name;
    int health;
    int attack;
    int defense;
    int experience;  // 新增經驗值屬性
    int level=1; 
    vector<Item*> inventory;
    Weapon* equippedWeapon;  // 裝備的武器

    // 初始化列表中沒有使用 nullptr，改為 NULL
    Character(string n, int h, int a, int d) 
        : name(n), health(h), attack(a), defense(d), equippedWeapon(NULL) {}

    void takeDamage(int damage) {
        health -= max(0, damage - defense);
    }

    void heal(int amount) {
        health += amount;
    }

    bool isAlive() {
        return health > 0;
    }

    void addItem(Item* item) {
        inventory.push_back(item);
    }

    void displayStats() {
        cout << name << " - 目前血量: " << health << " 攻擊力: " << attack << " 防禦力: " << defense << " 等級: " << level<< endl;
    }

    int getAttack() const {
        return attack + (equippedWeapon ? equippedWeapon->value : 0);
    }

    void displayInventory() {
        cout <<"\n#-------------#" ;
        cout << "\n|    背包    |"<<endl;
        cout << "#-------------#"<<endl<<endl;
        if (inventory.empty()) {
            cout << "背包是空的！\n";
        } else {
            for (int i = 0; i < inventory.size(); ++i) {
                cout << i + 1 << ". ";
                inventory[i]->display();
            }
        }
        cout << inventory.size() + 1 << ". 退出背包\n"<<endl;  // 加入退出選項
    }

    void useItem(int index) {
        if (index >= 0 && index < inventory.size()) {
            Item* item = inventory[index];
            if (item->name == "治療藥水") {
                heal(item->value);
                cout << name << " 使用了治療藥水，恢復了 " << item->value << " 生命！\n";
                inventory.erase(inventory.begin() + index);  // 使用藥水後從背包刪除
                delete item;  // 使用藥水後從背包刪除
            } else if (item->isWeapon()) {
                Weapon* weapon = dynamic_cast<Weapon*>(item);
                if (weapon) {
                    if (!weapon->isBroken()) {
                        if (equippedWeapon != NULL) {
                            cout << "你已經裝備了 " << equippedWeapon->name << "，無法裝備更多武器。\n";
                        } else {
                            equippedWeapon = weapon;  // 將選中的武器設為裝備
                            cout << name << " 裝備了 " << weapon->name << "，攻擊力提升了 " << weapon->value << "！\n";
                            inventory.erase(inventory.begin() + index);  // 裝備後從背包移除
                            // 注意：這裡 weapon 指標現在是 equippedWeapon，不要在這裡 delete
                        }
                } else {
                    cout << weapon->name << " 已經損壞，無法使用。\n";
                }
            }
        } else {
            cout << "無效的物品。"<<endl;
            cout <<endl ;
        }
    }
    };

    void displayEquippedWeapon() {
        if (equippedWeapon != NULL) {
            cout << "目前裝備的武器: " << equippedWeapon->name
                 << " (等級: " << equippedWeapon->level << ", "
                 << "耐久度: " << equippedWeapon->durability << ")\n";
        } else {
            cout << "目前沒有裝備武器。\n";
        }
    }
    
    // 新增升級功能
    void gainExperience(int exp) {
        experience += exp;
        cout << "獲得了 " << exp << " 點經驗值！"<<endl<<endl; 
        int experienceToNextLevel=level * 10; // 根據當前等級，升級所需的經驗值是等級 * 10
        if (experience >= experienceToNextLevel) {  // 升級條件
            experience -= experienceToNextLevel;  // 經驗值重置
            levelUp();
        
        }
    }

    void levelUp() {
        level++;
        attack += 3;  // 升級時增加攻擊力
        health += 5;  // 升級時增加血量
        defense += 2;  // 升級時增加防禦力
        cout << name << " 升級了！現在等級: " << level <<endl;
        cout<< "攻擊力: " <<attack <<" 生命值: " <<health<<" 防禦力: " <<defense<< endl<<endl;
    }
};

// 隨機生成敵人
Character generateEnemy(int playerLevel) {
    int enemyType = rand() % 100;
    int level = rand() % 3 + 1;
    if (playerLevel >= 12 && enemyType < 20) {
        return Character("宇宙無敵超級深淵惡魔-龍龍", 1000, rand() % 4 + 50, 20);  // 攻擊範圍 50 到 53
    }
    else if (playerLevel >= 9 &&enemyType < 30) {
        return Character("你的作業", 500, rand() % 400 + 40, 10);  // 攻擊範圍 40 到 43
    }
    else if (playerLevel >= 6 &&enemyType < 40) {
        return Character("林克", 250, rand() % 250 + 30, 15);  // 攻擊範圍 30 到 33
    }
    else if (playerLevel >= 3 &&enemyType < 50) {
        return Character("芙莉蓮花", 100, rand() % 100 + 15, 5);  // 攻擊範圍 15 到 18
    }
    else if (enemyType < 60) {
        return Character("哥布林士兵", 30, rand() % 4 + 10, 3);  // 哥布林士兵
    } 
    else {
        int health = (level == 1) ? 12 : (level == 2) ? 15 : 17;
        return Character("哥布林 (等級 " + to_string_custom(level) + ")", health, rand() % 3 + 8, 1);
    }
}

// 戰鬥系統
void battle(Character& player, Character& enemy) {
    cout << "一隻 " << enemy.name << " 出現了！\n"<<endl;

    while (player.isAlive() && enemy.isAlive()) {
        // 顯示雙方血量
        cout << player.name << " - 目前血量: " << player.health << "  |  "
             << enemy.name << " - 目前血量: " << enemy.health << endl;

        // 顯示攻擊選項
        cout << "\n";
        cout << "╔═════════════════╗"<<endl;
        cout << "║    選擇攻擊     ║"<<endl;
        cout << "╚═════════════════╝"<<endl;
        cout << "1. 普通攻擊\n";
        cout << "2. 強力攻擊\n";
        cout << "3. 使用武器攻擊";
        if (player.equippedWeapon != NULL) {
            cout << " (" << player.equippedWeapon->name << ")\n";
            cout << "4. 使用物品\n";
        } else {
            cout << "\n4. 使用物品\n";
        }
        cout << "選擇動作: ";
        int action;
        cin >> action;

        int damage = 0;
        if (action == 1) {
            damage = player.attack;
            cout << player.name << " 發起了普通攻擊，造成了 " << damage << " 點傷害。\n";
        } else if (action == 2) {
            damage = player.attack * 2;
            cout << player.name << " 發起了強力攻擊，造成了 " << damage << " 點傷害。\n";
        } else if (action == 3) {
            // 使用武器攻擊
            if (player.equippedWeapon != NULL) {
                player.equippedWeapon->use(); // 減少武器耐久度
                damage = player.getAttack();
                cout << player.name << " 使用 " << player.equippedWeapon->name << " 攻擊，造成了 " << damage << " 點傷害。\n";
                if (player.equippedWeapon->isBroken()) {
                    cout << player.equippedWeapon->name << " 損壞了！\n";
                    player.equippedWeapon = NULL; // 武器損壞後解除裝備
                }
            } else {
                cout << "你沒有裝備任何武器，無法使用武器攻擊。\n";
                continue; // 返回戰鬥選單
            }
        } else if (action == 4) {
            // 使用物品
            player.displayInventory();
            cout << "選擇一個物品來使用: ";
            int itemChoice;
            cin >> itemChoice;
            if (itemChoice > 0 && itemChoice <= player.inventory.size()) {
                player.useItem(itemChoice - 1);
            } else if (itemChoice == player.inventory.size() + 1) {
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
            cout << enemy.name << " 被擊敗了！\n"<<endl;
            // 當擊敗的是哥布林士兵，掉落劍
            if (enemy.name == "哥布林士兵") {
                player.addItem(new Weapon("劍", 15, 3, 2));  // 掉落劍（攻擊力 10, 耐久度 3, 等級 2）
                cout << "你撿到了 劍！\n";
            } else {
                // 若是其他敵人，掉落藥水
                Item loot("治療藥水", 20);
                player.addItem(new Potion("治療藥水", 20)); // 掉落藥水指標
                cout << "你撿到了 治療藥水！\n";
            }
            player.gainExperience(4 + player.level); // 每次擊敗敵人給予經驗值，並且隨著等級提升
            break;
        }

        // 敵人回擊
        damage = enemy.attack;
        player.takeDamage(damage);
        if(enemy.health!=0)
        cout << enemy.name << " 攻擊了你，造成了 " << damage << " 點傷害。"<<endl<<endl;
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
        cout << "║        主選單         ║"<<endl;
        cout << "╚═══════════════════════╝"<<endl;
    }
    cout << "1. 探索地下城\n2. 查看背包\n3. 查看裝備的武器\n4. 退出遊戲\n選擇一個選項: ";
}

int main() {
    srand(time(0));

    Character player("玩家", 21, 10, 3);
    bool firstTime = true;
    cout<<"歡迎來到地下城"<<endl;
    cout<< endl<<"等級: "<<1<<endl<<"攻擊力: " <<21 <<" 生命值: " <<10<<" 防禦力: " <<3<<endl;
    while (true) {
        showMenu(firstTime);
        int choice;
        cin >> choice;
        

        if (choice == 1) {
            cout <<"#-------------#";
            cout << "\n已進入地下城..."<<endl;
            Character enemy = generateEnemy(player.level);
            if (rand() % 3 == 0) {
                cout << "\n遇到敵人: " << enemy.name << endl;
                battle(player, enemy);
            } else {
                cout << "地下城一片寧靜... 什麼也沒有找到。\n"<<endl;;
            }
        } else if (choice == 2) {
            while (true) {
                player.displayInventory();
                cout << "選擇一個選項: ";
                int itemChoice;
                cin >> itemChoice;
                if (itemChoice == player.inventory.size() + 1) {
                    break;
                } else {
                    player.useItem(itemChoice - 1);
                }
            }
        } else if (choice == 3) {
            player.displayEquippedWeapon();  // 顯示目前所持武器
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
