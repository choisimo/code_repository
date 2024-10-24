//
// Created by csimo on 24. 10. 24.
//
#define s_num 916
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class quickSort
{
private:
    int seed = s_num % 1000;
public:
    int partition(int arr[], int p, int r);
    void quicksort(int arr[], int p, int r);
    void getSeed()
    {
        srand(this->seed);
    }
};

int quickSort::partition(int arr[], int p, int r)
{
    int x = arr[r];
    int i = p - 1;

    for (int j = p; j <= r - 1; j++)
    {
        if (arr[j] < x)
        {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[r]);
    return i + 1;
}

void quickSort::quicksort(int arr[], int p, int r){
    if(p < r){
        int q = partition(arr, p, r);
        quicksort(arr, p, q - 1);
        quicksort(arr, q + 1, r);
    }
}


int main()
{
    quickSort sort;
    sort.getSeed();

    int list[10000];


    for (int i = 0; i < 10000; i++)
    {
        list[i] = rand() % 1000000;
        /*if ((i >= 0 && i <= 9) || (i >= 2500 && i <= 2509) || (i >= 5000 && i <= 5009) || (i >= 7500 && i <= 7509) || (i >= 9990 && i <= 9999))
        {
            cout << "Before Sort: list[" << i << "] = " << list[i] << endl;
        }*/
    }
     clock_t s_time = clock();

    sort.quicksort(list, 0, 9999);

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
