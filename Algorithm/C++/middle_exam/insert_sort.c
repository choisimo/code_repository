//
// Created by csimo on 10/27/2024.
//
void insertionSort(int arr[], int n){
    for (int i = 1; i < n; i++) {
        int index = arr[i];
        int j;
        for (j = i - 1; j >= 0 && index < arr[j]; j--) {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = index;
    }
}

void insertionSort2(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        // 현재 index 위치
        int index = arr[i];

        int j = i - 1;
        for (; j >= 0 && index < arr[j]; j--)
        {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = index;
    }
}