//
// Created by csimo on 4/27/2024.
//
#include <iostream>
#include "Node.h"

template <typename T>
class doubleLinkedList{
private:
    Node<T>* head;
    Node<T>* prev;
    Node<T>* tail;
public:
    void add(T value){
        Node<T>* newNode = new Node<T>(value);

        newNode->setNext(nullptr);

        if(this->head == nullptr){
            this->head = newNode;
            this->tail = newNode;

            newNode->setPrev(nullptr);
        } else {
            newNode->setPrev(this->tail);
            this->tail->setNext(newNode);

            this->tail = newNode;
        }
    }

    T remove(){
        if(this->head == nullptr){
            throw std::out_of_range("empty list");
        }

        T value = this->tail->getValue();

        if(this->head == this->tail){
            delete this->head;
            this->head = nullptr;
            this->tail = nullptr;
        } else {
            tail = tail->getPrev();
            delete tail->getNext();
            tail->setNext(nullptr);
        }
    }

    T operator[](int index){
        Node<T>* p = this->head;
        for (int i = 0; i < index; i++) {
            p = p->getNext();
        }
        return p->getValue();
    }

    int size() {
        int size = 0;
        Node<T>* p = head;
        while (p != nullptr){
            size++;
            p = p->getNext();
        }
        return size;
    }

    void insert(int index, T value){
        Node<T>* newNode = new Node<T>(value);
        Node<T>* p = head;

        for(int i = 0; i < index-1; i++){
            p = p->getNext();
        }

        newNode->setNext(p->getNext());
        newNode->setPrev(p);

        newNode->getNext()->setPrev(newNode);
        newNode->getPrev()->setNext(newNode);
    }

    T _delete(int index){
       Node<T>* p = head;
       for (int i = 0; i < index; i++){
           p = p->getNext();
       }

       p->getPrev()->setNext(p->getNext());
       p->getNext()->setPrev(p->getPrev());

        T value = p->getValue();
        delete p;

        return value;
    }
};