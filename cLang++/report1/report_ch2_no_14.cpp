//
// Created by csimo on 9/12/2024.
//

/*
 * ch2 - no 14
 * 커피를 주문하는 간단한 C++ 프로그램을 작성해보자.
 * 커피 종류는 “에스프레소”, “아메리카노”, “카푸치노”의 3가지이며 가격은 각각 2000원, 2300원, 2500원이다.
 * 하루에 20,000원 이상 벌게 되면 카페를 닫는다. 실행 결과와 같이 작동하는 프로그램을 작성하라.
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
                cout << temp->name + " 주문 추가. 총 매출 : " << this->total_sale << endl;
                foundMenu = true;
                break;
            }
            temp = temp->next;
        }
        if (!foundMenu){
            cout << menu << " 이름의 메뉴는 없습니다. " << endl;
            cout << "가능한 메뉴 목록 출력 중 . . ." << endl;
            this->print_menu();
        }
    }

    void print_menu(){
        Node* temp = head;

        cout << "메뉴 목록 : " << endl;
        while (temp) {
            cout << "메뉴명 : " << temp->name << " | 메뉴 가격 : " << temp->price << endl;
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
    c.add_menu(2000, "에스프레소");
    c.add_menu(2300, "아메리카노");
    c.add_menu(2500,"카푸치노");

    char menu_name[20];

    while(c.isCafeOpen()){
        cout << "주문할 음료를 선택해주세요 : " << endl;
        cin.getline(menu_name, sizeof(menu_name));
        c.order_drink(menu_name);

        if (!c.isCafeOpen()){
            cout << "영업 끝났습니다. 다음에 다시 만나요. " << endl;
            break;
        }
    }

    return 0;
}