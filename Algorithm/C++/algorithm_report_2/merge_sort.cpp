//
// Created by csimo on 24. 10. 24.
//
#define s_num 916
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class mergeSort
{
private:
    int seed = s_num % 1000;
public:
    void getSeed()
    {
        srand(this->seed);
    }

    void merge(int arr[], int left, int mid, int right);
    void mergesort(int arr[], int left, int right);

};

void mergeSort::merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // 임시 배열 생성
    int *L = new int[n1];
    int *R = new int[n2];

    // L, R 배열로 값 복사
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // 병합
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // L의 남은 요소 복사
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    // R의 남은 요소 복사
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergeSort::mergesort(int arr[], int left, int right)
{
    if (left < right){
        int mid = left + (right - left) / 2;

        mergesort(arr, left, mid);
        mergesort(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}


int main()
{
    mergeSort sort;
    sort.getSeed();

    int list[10000];


    for (int i = 0; i < 10000; i++)
    {
        list[i] = rand() % 1000000;
    }

    clock_t s_time = clock();

    sort.mergesort(list, 0, 9999);

    clock_t e_time = clock();

    for (int i = 0; i < 10000; i++)
    {
        if ((i >= 0 && i <= 9) || (i >= 2500 && i <= 2509) || (i >= 5000 && i <= 5009) || (i >= 7500 && i <= 7509) || (i >= 9990 && i <= 9999))
        {
            cout << "After Sort: list[" << i << "] = " << list[i] << endl;
        }
    }

    cout << "quick sorting time : " << (double)(e_time - s_time) << " seconds" << endl;

    return 0;

}
