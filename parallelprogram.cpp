#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <omp.h>

using namespace std;
using namespace std::chrono;

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        
        #pragma omp parallel for shared(arr) private(i)
        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int partitionIndex = i + 1;
        
        #pragma omp task
        quickSort(arr, low, partitionIndex - 1);
        #pragma omp task
        quickSort(arr, partitionIndex + 1, high);
    }
}

int main() {
    int size = 10000000;
    vector<int> arr(size);

    srand(time(0));
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000;
    }

    auto start = high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single nowait
        quickSort(arr, 0, size - 1);
    }
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Parallel QuickSort took: " << duration.count() << " ms" << endl;

    return 0;
}
