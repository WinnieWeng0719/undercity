#include <iostream>
#include <ctime>   // For time()
#include <cstdlib> // For srand()
#include <string>

#include "Character.h"       // �ޤJ Character ���O
#include "./Item.h" // �T�O�]�t�ɮת����|���T           // �ޤJ Item ���O�Ψ�l���O
#include "./game_funtion.h"  // �ޤJ�C���禡
using namespace std;
int main() {
    srand(time(0));

    Character player("���a", 21, 10, 3);
    std::cout << "�w��Ө�a�U��" << std::endl;
    std::cout << std::endl << "���a��T" << std::endl <<
        "����: " << player.getLevelValue() << std::endl << "�����O: " << player.getAttackValue() << " �ͩR��: " << player.getHealth() << " ���m�O: " << player.getDefense() << std::endl;

    int consecutiveNothingFound = 0;

    while (true) {
        showMenu();
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "#-------------#";
            std::cout << "\n���a�i�J�a�U��..." << std::endl;

            // ��ܪ��a��T
            std::cout << "\n���a��T" << std::endl;
            std::cout << "����: " << player.getLevelValue() << std::endl
                << "�����O: " << player.getAttackValue()
                << " �ͩR��: " << player.getHealth()
                << " ���m�O: " << player.getDefense() << "\n" << std::endl;


            if (consecutiveNothingFound < 2) {
                if (rand() % 3 == 0) { // 33% ���v�J��ĤH
                    Character enemy = generateEnemy(player.getLevelValue());
                    std::cout << "\n�J��ĤH: " << enemy.getName() << std::endl;
                    battle(player, enemy);
                    consecutiveNothingFound = 0;
                }
                else {
                    std::cout << "�a�U���@�����R... \n" << std::endl;;
                    consecutiveNothingFound++;
                }
            }
            else {
                // �s��⦸���򳣨S���A�o���j��J��ĤH
                Character enemy = generateEnemy(player.getLevelValue());
                std::cout << "\n�J��ĤH: " << enemy.getName() << std::endl;
                battle(player, enemy);
                consecutiveNothingFound = 0;
            }
        }
        else if (choice == 2) {
            while (true) {
                if (player.getInventory().empty()) {
                    std::cout << "\n���~�M��O�Ū��I\n" << std::endl;
                    break; // �p�G�I�]�ŤF�A�N�����h�X�`��
                }
                player.displayItemList();
                std::cout << "��ܤ@�ӿﶵ: ";
                int itemChoice;
                std::cin >> itemChoice;
                if (itemChoice == player.getInventory().size() + 1) {
                    break;
                }
                else {
                    // �ϥΪ��~�ɡA�ݭn�P�_�ؼ�
                    Item* selectedItem = player.getInventory()[itemChoice - 1];
                    if (dynamic_cast<PoisonPotion*>(selectedItem) || dynamic_cast<ExplosionPotion*>(selectedItem)) {
                        std::cout << "���Ĥ��Ȧb�԰����ϥΡI\n"; // �o���Ĥ��u��b�԰�����ĤH�ϥ�
                    }
                    else {
                        player.useItem(itemChoice - 1, player); // ��ۤv�ϥ�
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
            std::cout << "�h�X�C���C���ݦA���D�Ԧa�U��!\n";
            break;
        }
        else {
            std::cout << "�L�Ī��ﶵ�A�ЦA�դ@���C\n";
        }

        if (!player.isAlive()) {
            break;
        }
    }

    return 0;
}