# include <iostream>
# include <cstdlib>
# include <ctime>
# include <omp.h>

using namespace std;

void min(int arr[],int n)
{
    int min_val=arr[0];

    #pragma omp paralle for reduction(min : min_val )
    for (int i=0;i<n;i++)
    {
        if (arr[i]<min_val)
        {
            min_val=arr[i];
        }
    }

    cout<<"Minimum Value in the array is : "<<min_val<<endl;
}

void max(int arr[],int n)
{
    int max_val=arr[0];

    #pragma omp parallel for reduction(max : max_val)
    for (int i=0;i<n;i++)
    {
        if (arr[i]>max_val)
        {
            max_val=arr[i];
        }
    }

    cout<<"Maximum value in the array is : "<<max_val<<endl;
}

void sum(int arr[],int n)
{
    int sum=0;

    #pragma omp parallel for reduction (+ : sum)
    for (int i=0;i<n;i++)
    {
        sum+=arr[i];
    }

    cout<<"Sum of the array elements is : "<<sum<<endl;
}

void avg(int arr[],int n)
{
    int sum=0;
    float avg=0;

    #pragma omp parallel for reduction (+ : sum)
    for (int i=0;i<n;i++)
    {
        sum+=arr[i];
    }

    avg=sum/n;
    cout<<"Average of the array elements is : "<<avg<<endl;   
}

int main()
{
    int n;

    cout<<"Enter the no. of elements of array : ";
    cin>>n;

    int arr[n];
    for (int i=0;i<n;i++)
    {
        arr[i]=rand() % 100;
    }

    cout<<"The array of elements is : [";
    for (int i=0;i<n;i++)
    {
        cout<<arr[i]<<", ";
    }
    cout<<"]"<<endl;

    max(arr,n);
    min(arr,n);
    sum(arr,n);
    avg(arr,n);
}