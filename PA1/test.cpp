#include "sort.hpp"
#include <bits/stdc++.h>
using namespace std;

bool compare_less(const double &lhs, const double &rhs) {
    return lhs < rhs;
}

int main(){
    double arr[5] ={5, 4, 3, 2, 1};
    vector<double> v(arr, arr+5);
    for(int i=0; i<(int)v.size(); i++) cout << v[i] << ' ';
    cout << endl;
    merge_sort(v, compare_less);
    for(int i=0; i<(int)v.size(); i++) cout << v[i] <<  ' ';
}