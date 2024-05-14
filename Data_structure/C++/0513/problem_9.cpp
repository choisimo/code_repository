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

    int sumAroundNode(Node* node, int n) {
        if (node == nullptr) {
            return 0; //return 타입이 int 형 이므로 0를 리턴한다.
        }
        int sum = node->data;  // 시작 노드 데이터 합
        Node *temp = node->prev; // 이전 노드로 이동
        int steps = n; // 이전 노드로 이동할 횟수 지정

        while (temp != nullptr && steps > 0) { // 이전 노드 탐색하며
            sum += temp->data;
            temp = temp->prev;  // 다음 이전으로 이동
            steps--;    // 지정 횟수 감소
        }

        temp = node->next;
        steps = n;


        while (temp != nullptr && steps > 0) {
            sum += temp->data;
            temp = temp->next; // 다음 노드로 이동
            steps--;          // 지정 횟수 감소
        }
        return sum;
    }
};