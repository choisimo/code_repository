//
// Created by csimo on 5/13/2024.
//
#include <iostream>
class Node {
public:
    int data;
    Node* prev;
    Node* next;
    Node(int data) : data(data), prev(nullptr), next(nullptr){
    }
};
class DoubleLinkedList {
public:
    Node *head;
    Node *tail;

    DoubleLinkedList() : head(nullptr), tail(nullptr) {
    }

    void add(int data){
        Node* newNode = new Node(data);
        if (this->head == nullptr){
            this->head = newNode;
            this->tail = newNode;
        }else{
            this->tail->next = newNode;
            newNode->prev = this->tail;
            this->tail = newNode;
        }
    }

    void reverse(){
        Node* current = this->head;
        Node* temp = nullptr;

        while (current != nullptr){
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }

        temp = this->head;
        this->head = this->tail;
        this->tail = temp;
    }
};