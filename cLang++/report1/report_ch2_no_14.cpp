//
// Created by csimo on 9/12/2024.
//

/*
 * ch2 - no 14
 * Ŀ�Ǹ� �ֹ��ϴ� ������ C++ ���α׷��� �ۼ��غ���.
 * Ŀ�� ������ �����������ҡ�, ���Ƹ޸�ī�롱, ��īǪġ�롱�� 3�����̸� ������ ���� 2000��, 2300��, 2500���̴�.
 * �Ϸ翡 20,000�� �̻� ���� �Ǹ� ī�並 �ݴ´�. ���� ����� ���� �۵��ϴ� ���α׷��� �ۼ��϶�.
* */

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class Node {
public:
    int price;
    string name;
    Node* next;
    Node(int price, string name) : price(price), name(name) ,next(nullptr){}
};

class cafe {
private:
    int total_sale;
    Node* head;
public:
    cafe(): head(nullptr), total_sale(0){}

    void add_menu(int price, string name){
        Node *newMenu = new Node(price, name);
        if (!head) {
            head = newMenu;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newMenu;
        }
    };

    void order_drink(const char* menu){
        Node* temp = head;
        bool foundMenu = false;
        while (temp) {
            if (strcmp(temp->name.c_str(), menu) == 0) {
                this->total_sale += temp->price;
                cout << temp->name + " �ֹ� �߰�. �� ���� : " << this->total_sale << endl;
                foundMenu = true;
                break;
            }
            temp = temp->next;
        }
        if (!foundMenu){
            cout << menu << " �̸��� �޴��� �����ϴ�. " << endl;
            cout << "������ �޴� ��� ��� �� . . ." << endl;
            this->print_menu();
        }
    }

    void print_menu(){
        Node* temp = head;

        cout << "�޴� ��� : " << endl;
        while (temp) {
            cout << "�޴��� : " << temp->name << " | �޴� ���� : " << temp->price << endl;
            temp = temp->next;
        }
        cout << endl;
    }

    bool isCafeOpen(){
        return total_sale < 20000;
    }

};
int main() {
    cafe c;
    c.add_menu(2000, "����������");
    c.add_menu(2300, "�Ƹ޸�ī��");
    c.add_menu(2500,"īǪġ��");

    char menu_name[20];

    while(c.isCafeOpen()){
        cout << "�ֹ��� ���Ḧ �������ּ��� : " << endl;
        cin.getline(menu_name, sizeof(menu_name));
        c.order_drink(menu_name);

        if (!c.isCafeOpen()){
            cout << "���� �������ϴ�. ������ �ٽ� ������. " << endl;
            break;
        }
    }

    return 0;
}