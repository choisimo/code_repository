//
// Created by csimo on 10/2/2024.
//

#include <iostream>
#include <ctime>
#define s_num 916
using namespace std;

class selection_sort{
private:
    int seed;
    int* arr;
    int size;
public:
    selection_sort() : size(1000) {
        this->arr = new int[this->size];
        this->seed = s_num % 1000;
        srand(this->seed);
        for (int i = 0; i < this->size; i++) {
            this->arr[i] = rand() % 100000;
        }
    }

    // 선택 정렬 구현
    void sort(){
        int i, j, min, temp;
        for (i = 0; i < this->size - 1; i++) {
            min = i;
            for (j = i + 1; j < this->size; j++) {
                if (arr[j] < arr[min]) {
                    min = j;
                }
            }
            if (min != i){
                temp = arr[i];
                arr[i] = arr[min];
                arr[min] = temp;
            }
        }
    }

    // 배열 출력 함수
    void printSort() {
        cout << "Sorted arrays [selection sort]" << endl;
        cout << "-----------------------------" << endl;
        for (int i = 0; i < this->size; i++) {
            // 일부 구간만 출력 (0~9, 500~509, 990~999)
            if ((i >= 0 && i <= 9) || (i >= 500 && i <= 509) || (i >= 990 && i <= 999)) {
                cout << "list[" << i << "] = " << arr[i] << endl;
            }
            if (i == 9 || i == 509 || i == 999){
                cout << "-----------------------------" << endl;
            }
        }
    }

    ~selection_sort(){
        delete[] this->arr;
    }
};

int main(){
    clock_t stime = clock();
    selection_sort selectSort;
    selectSort.sort();
    selectSort.printSort();
    clock_t etime = clock();
    double interval = (double)(etime - stime) / CLOCKS_PER_SEC;
    cout << "end time - start time = " << interval << endl;
}