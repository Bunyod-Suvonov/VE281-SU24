#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP

#include <vector>
#include <stdlib.h>
#include <functional>
#include <iostream>

template<typename T, typename Compare>
void bubble_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    if((int)vector.size()<2) return;
    for(int i= (int) vector.size()-2; i>=0; i--)
        for(int j=0; j<=i; j++)
            if(comp(vector[j+1], vector[j]))
                std::swap(vector[j], vector[j+1]);
}

template<typename T, typename Compare>
void insertion_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    if((int)vector.size()<2) return;
    for(int i=1; i< (int) vector.size(); i++){
        T key = vector[i];
        int j=i-1;
        while(j>=0 && comp(key, vector[j])){
            vector[j+1] = vector[j];
            j--;
        }
        vector[j+1] = key;
    }
}

template<typename T, typename Compare>
void selection_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    T min;
    auto it = vector.begin();
    for(auto i=vector.begin(); i!=vector.end(); ++i){
        min = *i;
        it = i;
        for(auto j=i+1; j!=vector.end(); j++){
            if(comp(*j, min)){
                min = *j;
                it=j;
            }
        }
        std::swap(*i, *it);
    }
}




// merge_sort helpers

template<typename T, typename Compare>
void merge(std::vector<T> &a, int left, int mid, int right, Compare comp) {
    // copy parts of array to new ones
    int size_l = mid-left+1;
    int size_r = right-mid;
    std::vector<T> left_side(size_l);
    std::vector<T> right_side(size_r);

    for(int i=0; i < size_l; i++){
        left_side[i] = a[left+i];
    }
    for(int i=0; i < size_r; i++){
        right_side[i] = a[mid+1+i];
    }

    // merge and copy the elements of arrays to original array through iteration
    int l = 0;
    int r = 0;
    int curr=left;
    while (l < size_l && r < size_r) {
        if (comp(right_side[r], left_side[l])) {
            a[curr] = right_side[r];
            r++;
        } else {
            a[curr] = left_side[l];
            l++;
        }
        curr++;
    }

    // copy the remaining elements to original array
    while (l < size_l) {
        a[curr] = left_side[l];
        l++;
        curr++;
    }
    while (r < size_r) {
        a[curr] = right_side[r];
        r++;
        curr++;
    }
}

template<typename T, typename Compare>
void divide(std::vector<T> &a, int left, int right, Compare comp) {
    if (left >= right) return;
    int mid = left+(right-left)/ 2;
    divide(a, left, mid, comp);
    divide(a, mid + 1, right, comp);
    merge(a, left, mid, right, comp);
}

template<typename T, typename Compare>
void merge_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    if((int)vector.size()<2) return;
    divide(vector, 0, (int) vector.size()-1, comp);
}

// quicksort implementations

template<typename T, typename Compare>
int Partition(std::vector<T> &vector, int low, int high, Compare comp = std::less<T>()){
    int pivot = high;
    int first = low-1;
    for(int second=low; second<high; second++){
        if(comp(vector[second], vector[pivot])){
            first++;
            std::swap(vector[first], vector[second]);
        }
    }
    std::swap(vector[first+1], vector[pivot]);
    return first+1;
}

template<typename T, typename Compare>
void quick_sort_helper(std::vector<T> &vector, int low, int high, Compare comp = std::less<T>()){
    if(low<high){
        int pivot = Partition(vector, low, high, comp);
        quick_sort_helper(vector, low, pivot-1, comp);
        quick_sort_helper(vector, pivot+1, high, comp);
    }
}

template<typename T, typename Compare>
void quick_sort_extra(std::vector<T> &vector, Compare comp = std::less<T>()) {
    if((int)vector.size()<2) return;
    std::vector<T> clone(vector.begin(), vector.end());
    quick_sort_helper(clone, 0, (int) clone.size()-1, comp);
    vector = clone;
}

template<typename T, typename Compare>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = std::less<T>()) {
    if((int)vector.size()<2) return;
    quick_sort_helper(vector, 0, (int) vector.size()-1, comp);
}

#endif //VE281P1_SORT_HPP