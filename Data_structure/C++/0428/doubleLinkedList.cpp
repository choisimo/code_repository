/*
//
// Created by csimo on 4/28/2024.
//

#include <iostream>



int main() {
    LinkedList<int> list;
    list.add(1);
    list.add(2);
    list.add(30);
    list.add(4);
    list.add(5);

    list.printState();

    for(int i=0; i < list.size(); i++)
        std::cout << list[i] << " ";
    std::cout << std::endl;

    list._delete(2);

    list.printState();

    for(int i=0; i < list.size(); i++)
        std::cout << list[i] << " ";
    std::cout << std::endl;

    return 0;
}*/
