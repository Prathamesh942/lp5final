#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;

float bubbleSort(vector<int> arr){
    auto t1 = high_resolution_clock::now();
    int n=arr.size();
    for(int i=0; i<n; i++){
        for(int j=1; j<n; j++){
            if(arr[j]<arr[j-1]){
                int temp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1]=temp;
            }
        }
    }
    auto t2 = high_resolution_clock::now();
    return (t2-t1).count();
}

float bubbleSortp(vector<int> arr){
    auto t1 = high_resolution_clock::now();
    int n=arr.size();
    for(int i=0; i<n; i++){
        #pragma omp parallel for
        for(int j=1; j<n; j+=2){
            if(arr[j]<arr[j-1]){
                int temp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1]=temp;
            }
        }
        #pragma omp parallel for
        for(int j=2; j<n; j+=2){
            if(arr[j]<arr[j-1]){
                int temp = arr[j];
                arr[j] = arr[j-1];
                arr[j-1]=temp;
            }
        }
    }
    auto t2 = high_resolution_clock::now();
    return (t2-t1).count();
}

void merge(vector<int>& arr,int l,int m,int r){
    int n = r-l+1;
    vector<int> temp(n);
    int p1=l,p2=m+1,p=0;
    while(p1<=m && p2<=r){
        if(arr[p1]<arr[p2]){
            temp[p] = arr[p1];
            p1++;
        }else{
            temp[p]=arr[p2];
            p2++;
        }
        p++;
    }
    while(p1<=m){
        temp[p] = arr[p1];
        p++;
        p1++;
    }
    while(p2<=r){
        temp[p] = arr[p2];
        p++;
        p2++;
    }
    for(int i=0; i<n; i++) arr[l+i]=temp[i];
}

void mergeSort(vector<int> arr, int l, int r){
    if(l<r){
        int m=l+(r-l)/2;
        mergeSort(arr,l,m);
        mergeSort(arr,m+1,r);
        merge(arr,l,m,r);
    }
}

void mergeSortp(vector<int> arr, int l, int r){
    if(l<r){
        int m=l+(r-l)/2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(arr,l,m);

            #pragma omp section
            mergeSort(arr,m+1,r);
        }
        merge(arr,l,m,r);
    }
}
int main(){
    int n=10000;
    vector<int> arr1;
    for(int i=0; i<n; i++){
        arr1.push_back(rand()%1000);
    }
    cout<<bubbleSort(arr1)/bubbleSortp(arr1)<<"\n";
    auto t1 = high_resolution_clock::now();
    mergeSort(arr1,0,n-1);
    auto t2 = high_resolution_clock::now();
    float mt1 = (t2-t1).count();
    t1 = high_resolution_clock::now();
    mergeSortp(arr1,0,n-1);
    t2 = high_resolution_clock::now();
    float mt2 = (t2-t1).count();
    cout<<mt1/mt2;
}