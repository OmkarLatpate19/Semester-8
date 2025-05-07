# include <iostream>
# include <cstdlib>
# include <omp.h>

using namespace std;

void sequential_bubblesort(int arr[],int n)
{
    for (int i=0;i<n-1;i++)
    {
        for (int j=0;j<n-i-1;j++)
        {
            if (arr[j]>arr[j+1])
            {
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void parallel_bubblesort(int arr[],int n)
{
    for (int i=0;i<n;i++)
    {
        int start=i%2;
        # pragma omp parallel for
        for (int j=start;j<n-1;j+=2)
        {
            if (arr[j] > arr[j+1])
            {
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void merge(int arr[],int l,int m,int r)
{
    int n1=m-l+1;
    int n2=r-m;

    int *L=new int[n1];
    int *R=new int[n2];

    for (int i=0;i<n1;i++)
    {
        L[i]=arr[l+i];
    }

    for(int j=0;j<n2;j++)
    {
        R[j]=arr[m+1+j];
    }

    int i=0,j=0,k=l;

    while (i<n1 && j<n2){
        arr[k++]=(L[i] <= R[j]) ? L[i++] : R[j++];
    }
    while (i<n1)
        arr[k++]=L[i++];
    while (j<n2)
        arr[k++]=R[j++];

    delete[] L;
    delete[] R;
}

void sequential_mergesort(int arr[],int l,int r)
{
    if (l<r)
    {
        int m=l+(r-l)/2;
        sequential_mergesort(arr,l,m);
        sequential_mergesort(arr,m+1,r);
        merge(arr,l,m,r);
    }
}

void parallel_mergesort(int arr[],int l,int r,int d=0)
{
    if (l<r)
    {
        int m=l+(r-l)/2;
        if (d>4)
        {
            parallel_mergesort(arr,l,m,d+1);
            parallel_mergesort(arr,m+1,r,d+1);
        }
        else
        {
            # pragma omp parallel sections
            {
                # pragma omp section
                parallel_mergesort(arr,l,m,d+1);
                # pragma omp section
                parallel_mergesort(arr,m+1,r,d+1);
            }
        }
        merge(arr,l,m,r);
    }
}

int main()
{
    int n;
    cout<<"Enter the size of the array : "<<endl;
    cin>>n;

    int *original=new int[n];

    for (int i=0;i<n;i++)
    {
        original[i]=rand()%100;
    }

    int *arr1=new int[n];
    int *arr2=new int[n];
    int *arr3=new int[n];
    int *arr4=new int[n];

    for (int i=0;i<n;i++)
    {
        arr1[i]=arr2[i]=arr3[i]=arr4[i]=original[i];
    }

    double start,end;

    start=omp_get_wtime();
    sequential_bubblesort(arr1,n);
    end=omp_get_wtime();
    double seq_bubble_time=end-start;

    start=omp_get_wtime();
    parallel_bubblesort(arr2,n);
    end=omp_get_wtime();
    double par_bubble_time=end-start;

    start=omp_get_wtime();
    sequential_mergesort(arr3,0,n-1);
    end=omp_get_wtime();
    double seq_merge_time=end-start;

    start=omp_get_wtime();
    parallel_mergesort(arr4,0,n-1);
    end=omp_get_wtime();
    double par_merge_time=end-start;

    cout<<"Performance Comparisons : "<<endl;
    cout<<"Sequential Bubble Sort : "<<seq_bubble_time<<" seconds."<<endl;
    cout<<"Parallel Bubble Sort : "<<par_bubble_time<<" seconds."<<endl;
    cout<<"Sequential Merge Sort : "<<seq_merge_time<<" seconds."<<endl;
    cout<<"Parallel Merge Sort : "<<par_merge_time<<" seconds."<<endl;

    delete[] original;
    delete[] arr1;
    delete[] arr2;
    delete[] arr3;
    delete[] arr4;

}