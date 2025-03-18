//
// Created by csimo on 24. 10. 29.
//

#include <iostream>
#include <cstring>
using namespace std;

class quickSort
{
private:
    int *arr,size,p,q,r;
public:
    quickSort();
    quickSort(int* a, int s) : arr(a), size(s) {};
    void swap(int*d, int*t) {int temp = *d; *d = *t; *t = temp;}
    int partition(int p, int r);
    void sort(int p, int r);
    void isStringNumeric(const string& str);
};

int quickSort::partition(int p, int r)
{
    if (p >= r) return -1;

    int pivot = arr[r], i = p - 1;

    for (int j = p; j < r; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            this->swap(&arr[i], &arr[j]);
        }
    }
    this->swap(&arr[i + 1], &arr[r]);
    return i + 1;
}

void quickSort::sort(int p, int r)
{
    if (p >= r) return;
    int q = partition(p, r);
    sort(p, q - 1);
    sort(q + 1, r);
}

void quickSort::isStringNumeric(const string& str)
{
    for (char ch : str) if (!isdigit(ch))ch = '';
}

int main()
{
    string line;
    quickSort newSort;

    do
    {
        if (line.length() < 1) break;
        getline(cin, line, '\n');
    } while (line != "exit");



}