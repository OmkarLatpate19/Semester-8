#include <iostream>
#include <cstdlib>
#include <omp.h>

using namespace std;

// Sequential Bubble Sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort using odd-even transposition
void parallelBubbleSort(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        int start = i % 2;
        #pragma omp parallel for
        for (int j = start; j < n - 1; j += 2) {
            // s

            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }

         
        }
    }
}

// Merge function
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = new int[n1];
    int *R = new int[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2)
        arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

// Sequential / Parallel Merge Sort


void mergeSort(int arr[], int l, int r,int d=0) {
    if (l < r) {
        int m = l + (r - l) / 2;

        if (d>4) {
            // Small subarray – do it sequentially
            mergeSort(arr, l, m,d+1);
            mergeSort(arr, m + 1, r,d+1);
        } else {
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSort(arr, l, m,d+1);
                #pragma omp section
                mergeSort(arr, m + 1, r,d+1);
            }
        }

        merge(arr, l, m, r);
    }
}


int main() {
    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    int *original = new int[n];
    for (int i = 0; i < n; ++i)
        original[i] = rand() % 100;

    // Create copies
    int *arr1 = new int[n]; // Sequential Bubble
    int *arr2 = new int[n]; // Parallel Bubble
    int *arr3 = new int[n]; // Sequential Merge
    int *arr4 = new int[n]; // Parallel Merge

    for (int i = 0; i < n; ++i)
        arr1[i] = arr2[i] = arr3[i] = arr4[i] = original[i];

    double start, end;

    // Sequential Bubble Sort
    start = omp_get_wtime();
    bubbleSort(arr1, n);
    end = omp_get_wtime();
    double time_seq_bubble = end - start;
   
   
    // Parallel Bubble Sort
    start = omp_get_wtime();
    parallelBubbleSort(arr2, n);
    end = omp_get_wtime();
    double time_par_bubble = end - start;

    // Sequential Merge Sort
    start = omp_get_wtime();
    mergeSort(arr3, 0, n - 1);
    end = omp_get_wtime();
    double time_seq_merge = end - start;

    // Parallel Merge Sort
    start = omp_get_wtime();

    mergeSort(arr4, 0, n - 1);
 
    end = omp_get_wtime();
    double time_par_merge = end - start;

    // Results
    cout << "\nPerformance Comparison (in seconds):\n";
    cout << "Sequential Bubble Sort: " << time_seq_bubble << endl;
    cout << "Parallel Bubble Sort:   " << time_par_bubble << endl;
    cout << "Sequential Merge Sort:  " << time_seq_merge << endl;
    cout << "Parallel Merge Sort:    " << time_par_merge << endl;

    delete[] original;
    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
    delete[] arr4;

   
}