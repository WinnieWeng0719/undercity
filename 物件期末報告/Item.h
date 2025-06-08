#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

// 前向宣告 Character 類別，因為 Item 類別中的 applyEffect 函式會使用到 Character 類別
class Character;

class Item {
private:
    std::string name_;
    int value_;    // 物品效果值
    int level_;    // 物品等級

public:
    Item(std::string n, int v, int l = 1);
    virtual void display() const;
    virtual bool isWeapon() const;
    virtual void applyEffect(Character& target, Character& attacker); // 需要傳入目標角色和攻擊者

    // Getter 方法
    std::string getName() const;
    int getValue() const;
    int getLevel() const;

    virtual ~Item(); // 虛擬析構函式
};

#endif // ITEM_H