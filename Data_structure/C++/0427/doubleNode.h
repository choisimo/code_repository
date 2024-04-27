//
// Created by csimo on 4/27/2024.
//

#ifndef CODE_REPOSITORY_DOUBLENODE_H
#define CODE_REPOSITORY_DOUBLENODE_H

template <typename T>
class Node{
private:
    T value;
    Node* next;
    Node* prev;

public:
    Node(T value){
        this->setValue(value);
        this->setNext(nullptr);
    }

    void setvalue(T value){
        this->value = value;
    }

    void setNext(Node* next){
        this->next = next;
    }

    void setPrev(Node* prev){
        this->prev = prev;
    }

    T getValue() const {
        return this->value;
    }

    Node* getNext() const {
        return this->next;
    }

    Node* getPrev() const {
        return this->prev;
    }
};
#endif //CODE_REPOSITORY_DOUBLENODE_H
