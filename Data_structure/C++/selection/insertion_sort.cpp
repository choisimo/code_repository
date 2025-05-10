#include <iostream>

// 삽입 정렬 예시 

int main(){
    
    int i, j, index, temp;
    
    int array[10] = {1, 10, 5, 8, 7, 6, 4, 3, 2, 9};

    for (i = 0; i < 10; i++){
        for (j = 0; j < i+1; j++){
            if (array[i] > array[j]){
                temp = array[j];
                array[j] = array[i];
                array[i] = temp;
            }
        }
    }
    for (i=9; i >= 0 ; i--)
    std::cout << array[i] << " ";
}