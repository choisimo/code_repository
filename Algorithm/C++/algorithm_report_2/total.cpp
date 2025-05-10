//
// Created by csimo on 24. 10. 24.
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = new int[n1];
    int *R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    const int SIZE = 10000;
    int list[SIZE], listCopy[SIZE];

    // Seed 설정
    int s_num = 123456; // 학번 예시
    int seed = s_num % 1000;
    srand(seed);

    // 랜덤 데이터 생성
    for (int i = 0; i < SIZE; i++) {
        list[i] = rand() % 1000000;
    }

    // 배열 복사
    for (int i = 0; i < SIZE; i++) {
        listCopy[i] = list[i];
    }

    // Merge Sort
    auto start = high_resolution_clock::now();
    mergeSort(list, 0, SIZE - 1);
    auto stop = high_resolution_clock::now();
    auto mergeDuration = duration_cast<seconds>(stop - start);

    cout << "Merge Sort Results:" << endl;
    cout << "list[0]...list[9]: ";
    for (int i = 0; i < 10; i++) cout << list[i] << " ";
    cout << endl;
    cout << "list[2500]...list[2509]: ";
    for (int i = 2500; i < 2510; i++) cout << list[i] << " ";
    cout << endl;
    cout << "list[5000]...list[5009]: ";
    for (int i = 5000; i < 5010; i++) cout << list[i] << " ";
    cout << endl;
    cout << "list[7500]...list[7509]: ";
    for (int i = 7500; i < 7510; i++) cout << list[i] << " ";
    cout << endl;
    cout << "list[9990]...list[9999]: ";
    for (int i = 9990; i < 10000; i++) cout << list[i] << " ";
    cout << endl;
    cout << "Merge Sort Time: " << mergeDuration.count() << " seconds" << endl;

    // Quick Sort
    start = high_resolution_clock::now();
    quickSort(listCopy, 0, SIZE - 1);
    stop = high_resolution_clock::now();
    auto quickDuration = duration_cast<seconds>(stop - start);

    cout << "\nQuick Sort Results:" << endl;
    cout << "list[0]...list[9]: ";
    for (int i = 0; i < 10; i++) cout << listCopy[i] << " ";
    cout << endl;
    cout << "list[2500]...list[2509]: ";
    for (int i = 2500; i < 2510; i++) cout << listCopy[i] << " ";
    cout << endl;
    cout << "list[5000]...list[5009]: ";
    for (int i = 5000; i < 5010; i++) cout << listCopy[i] << " ";
    cout << endl;
    cout << "list[7500]...list[7509]: ";
    for (int i = 7500; i < 7510; i++) cout << listCopy[i] << " ";
    cout << endl;
    cout << "list[9990]...list[9999]: ";
    for (int i = 9990; i < 10000; i++) cout << listCopy[i] << " ";
    cout << endl;
    cout << "Quick Sort Time: " << quickDuration.count() << " seconds" << endl;

    return 0;
}
