//
// Created by csimo on 5/13/2024.
//

#include <iostream>
class Node {
public:
    int data;
    Node* prev;
    Node* next;
    Node(int data) : data(data), prev(nullptr), next(nullptr)
    {}
};
class DoubleLinkedList {
public:
    Node* head;
    Node* tail;
    DoubleLinkedList() : head(nullptr), tail(nullptr) {}
    void add(int data) {
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }
    void remove(int data) {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == data) {
                if (current->prev != nullptr)
                    current->prev->next = current->next;
                if (current->next != nullptr)
                    current->next->prev = current->prev;
                if (current == head)
                    head = current->next;
                if (current == tail)
                    tail = current->prev;

                delete current;
            }
            current = current->next;
        }
    }
    void print() {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};