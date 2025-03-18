//
// Created by csimo on 4/27/2024.
//

#ifndef NODE_H
#define NODE_H

template <typename T>
class Node {
private:
    T data;
    Node* next;
public:
    T getData() const {
        return this->data;
    }
    Node* getNext() const{
        return this->next;
    }
    void setData(T data){
        this->data = data;
    }
    void setNext(Node* next){
        this->next = next;
    }
};

#endif