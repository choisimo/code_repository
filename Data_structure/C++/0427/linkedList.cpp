//
// Created by csimo on 4/27/2024.
//
#include "Node.h"

template <typename T>
class linkedList {
private:
    Node<T>* head;
public:
    linkedList() : head(nullptr){}

    ~linkedList(){
        Node<T>* p = head;
        while(p != nullptr){
            Node<T> *next = p->getNext();
            delete p;
            p = next;
        }
    }

    void add(T data){
        Node<T> *newNode = new Node<T>();
        newNode->setData(data);
        newNode->setNext(nullptr);

        if (head == nullptr) {
            head = newNode;
        } else {
            Node<T>* p = head;
            while(p->getNext() != nullptr){
                p = p->getNext();
            }
            p->setNext(newNode);
        }
    }

    void remove(){
        if (head == nullptr){
            return;
        }

        Node<T>* p = head;
        Node<T>* prev = nullptr;

        while(p->getNext() != nullptr){
            prev = p;
            p = p->getNext();
        }
        if (prev == nullptr){
            head = nullptr;
        } else {
            prev->setNext(nullptr);
        }
        delete p;
    }

    bool contains(T value){
        Node<T>* p = head;
        while(p != nullptr){
            if(p->getValue() ==  value){
                return true;
            }
            p = p->getNext();
        }
        return false;
    }
};