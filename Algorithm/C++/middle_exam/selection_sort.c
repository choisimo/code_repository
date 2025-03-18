//
// Created by csimo on 10/27/2024.
//
void selectionSort(int arr[], int n){
    int i, j, min, temp;
    // 마지막 요소는 자동으로 올바른 위치에 놓이게 된다
    for (i = 0; i < n - 1; i++) {
        min = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min])
                min = j;
        temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }
}