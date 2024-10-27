//
// Created by csimo on 10/27/2024.
//
void bubbleSort(int list[], int n){
    int i, temp;
    int sorted = 0;

    for (int last = n - 1; last > 0; last--) {
        sorted = 1;
        for (i = 0; i < last; i++) {
            if (list[i] > list[i + 1]){
                temp = list[i];
                list[i] = list[i + 1];
                list[i + 1] = temp;
                sorted = 0;
            }
        }
        if (sorted == 1) break;
    }
}