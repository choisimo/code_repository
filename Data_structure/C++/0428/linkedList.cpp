/*
//
// Created by csimo on 4/29/2024.
//

template <typename T>
class linkedList{
private:
    Node<T>* head;
    Node<T>* tail;
public:
    linkedList(){
        this->setHead(nullptr);
        this->setTail(nullptr);
    }
    ~linkedList(){
        Node<T>* p = head;
        while(p != nullptr){
            Node <T> *next = p->getNext();
            delete p;
            p = next;
        }
    }
    void add(T value){
        Node <T> *newNode = new Node<T>(value);
        if(this->head == nullptr){
            this->head = newNode;
            this->tail = newNode;
        } else {
            this->tail->setNext(newNode);
            this->tail = newNode;
        }
    }
    T remove(){
        if(this->head == nullptr){
            throw std::out_of_range("empty list");
        }
        T value = this->head->getValue();
        Node <T> *tmp = this->tail;
        delete tmp;

        Node<T>* p = head;
        while(p->getNext() != nullptr){
            p = p->getNext();
        }
        this->tail = p;
        return value;
    }
};*/