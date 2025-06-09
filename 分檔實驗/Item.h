#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

// 前向宣告 Character 類別，因為 Item 類別的 applyEffect 函式會用到它
class Character;

// 物品類別
class Item {
private:
    std::string name_;
    int value_;    // 物品效果值
    int level_;    // 物品等級

public:
    Item(std::string n, int v, int l = 1);
    virtual void display() const;
    virtual bool isWeapon() const;

    // 新增虛擬函式來應用物品效果，需要傳入目標角色和攻擊者
    // 預設不做任何事，由子類別覆寫
    virtual void applyEffect(Character& target, Character& attacker);

    // Getter 方法
    std::string getName() const;
    int getValue() const;
    int getLevel() const;

    // 虛擬析構函式，確保正確釋放記憶體
    virtual ~Item();
};

#endif // ITEM_H