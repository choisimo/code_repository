//
// Created by csimo on 24. 10. 29.
//
#include <stdio.h>
#include <stdlib.h>


// key 는 노드에 저장된 값,
// left -> 왼쪽 자식
// right -> 오른쪽 자식
typedef struct Node
{
    int key;
    struct Node* left;
    struct Node* right;
} Node;



// 이진 검색 트리 search
// t 는 현재 노드를 가리키는 포인터
Node* treeSearch(Node* t, int key)
{
    if (t == NULL || t->key == key)
    {
        return t;
    }

    if (key < t->key)
    {
        return treeSearch(t->left, key);
    } else
    {
        return treeSearch(t->right, key);
    }
}




// 새로운 노드 생성 함수
Node* createNode(int key)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* treeInsert(Node *t, int x)
{
    // t가 NULL 인 경우 새로운 노드를 생성하여 반환
    if (t == NULL)
    {
        return createNode(x);
    }

    if (x < t->key)
    {
        t->left = treeInsert(t->left, x);
    } else
    {
        t->right = treeInsert(t->right, x);
    }

    return t;
}