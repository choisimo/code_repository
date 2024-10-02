//
// Created by csimo on 10/2/2024.
//
#include <iostream>
#include <time.h>
#define s_num 916
using namespace std;

class insertionSort{
private:
    int seed;
    int* arr;
    int size;
public:
    insertionSort() : size(1000) {
        this->seed = s_num % 1000;
        srand(this->seed);

        // 배열 동적 할당
        this->arr = new int[this->size];

        for (int i = 0; i < this->size; i++) {
            this->arr[i] = rand() % 100000;
        }
    }

    // 삽입 정렬 구현
    void sort(){
        for (int i = 1; i < size; i++) {
            int key = this->arr[i];

            // 중간 0 ~ 9, 500 ~ 509, 990 ~ 999 출력
            if (i >= 0 && i <= 9 || i >= 500 && i <= 509 || i >= 990 && i <= 999){
                cout << "list [" <<  i << "] " << arr[i] << endl;
            }

            int j = i - 1;
            // key 보다 큰 요소를 한 칸씩 오른쪽 이동
            while(j >= 0 && key < this->arr[j]){
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }


    ~insertionSort(){
        delete[] arr;
    }
};

int main(){
    insertionSort newSort;
    // 시작 시간
    clock_t start_time = clock();
    // 삽입 정렬 수행
    newSort.sort();
    // 끝나는 시간
    clock_t end_time = clock();

    double interval = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    cout << "spent time for sorting : " << interval << endl;
    return 0;
}

