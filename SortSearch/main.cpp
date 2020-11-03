#include <iostream>

using namespace std;

template <class T>
void printElements(T* arr, int len) {
    for(int i = 0; i < len; i++) {
        cout << arr[i] << ", ";
    }
    cout << endl;
}

template <class T>
void swap(T* ele1, T* ele2) {
    int temp = *ele1;
    *ele1 = *ele2;
    *ele2 = temp;
}

template <class T>
void bubbleSort(T* arr, int len) {
    bool notSorted = true;
    while(notSorted) {
        notSorted = false;
        for(int i = 0; i < len-1; i++) {
            if(arr[i] > arr[i+1]) {
                swap(&arr[i+1],&arr[i]);
                notSorted = true;
            }
        }
        printElements(arr,len);
    }
}

template <class T>
void selectionSort(T* arr, int len) {
    for(int i = 0; i < len-1; i++) {
        int best = i;
        for(int j = i+1; j < len; j++) {
            if(arr[j] < arr[best]) {
                best = j;
            }
        }
        swap(&arr[i],&arr[best]);
        printElements(arr,len);
    }
}

template <class T>
void insertionSort(T* arr, int len) {
    for(int i = 1; i < len; i++) {
        int j = i-1;
        while(j >= 0 && *(arr+j) > *(arr+j+1)) {
            swap(&arr[j+1],&arr[j]);
            j--;
        }
        printElements(arr,len);
    }
}

template <class T>
int partition(T* arr, int left, int right) {
    int pivot = right;
    int i = left-1;
    while(left <= right) {
        if(arr[left] < arr[pivot]) {
            i++;
            swap(&arr[i],&arr[left]);
        }
        left++;
    }
    swap(&arr[i+1],&arr[pivot]);
    return i+1;
}

template <class T>
void quickSort(T* arr, int left, int right) {
    if(left < right) {
        int pivot = partition(arr,left,right);
        quickSort(arr,left,pivot-1);
        quickSort(arr,pivot+1,right);
    }
}

template <class T>
void quickSort(T* arr, int len) {
    quickSort(arr,0,len-1);
}

template <class T>
int binarySearch(T* arr, int len, T num) {
    int high = len-1;
    int low = 0;
    while(low <= high) {
        int mid = (high+low)/2;
        if(arr[mid] < num) {
            low = mid+1;
        } else if(arr[mid] > num) {
            high = mid-1;
        } else {
            return mid;
        }
    }
    return -1;
}

int main()
{
    int len = 5;
    float arr[] = {34,12,-1,0,7,4};

    printElements(arr,len);

    //quickSort<float>(arr,len);
    cout << partition(arr,0,len-1);

    printElements(arr,len);

    cout << binarySearch<float>(arr,5,5) << endl;
    cout << binarySearch<float>(arr,5,3) << endl;
    cout << binarySearch<float>(arr,5,1) << endl;

    return 0;
}
