#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int parent;
    int rank;
} Node;

void makeSet(Node nodes[], int n) {
    for (int i = 0; i < n; i++) {
        nodes[i].parent = i;
        nodes[i].rank = 0;
    }
}

int findSet(Node nodes[], int x){
    if (nodes[x].parent != x){
        nodes[x].parent = findSet(nodes, nodes[x].parent);
    }
    // if (nodes[x].parent == x){  자기 자신이 부모인 경우
    return nodes[x].parent;
}

void unionSet(Node nodes[], int x, int y){
    x = findSet(nodes, x);
    y = findSet(nodes, y);

    // rank 값이 큰 노드를 부모로 설정
    if (nodes[x].rank > nodes[y].rank){
        nodes[y].parent = x;
    }
    // rank 값이
    else
    {
        nodes[x].parent = y;
        if (nodes[x].rank == nodes[y].rank){
            nodes[y].rank++;
        }
    }
}