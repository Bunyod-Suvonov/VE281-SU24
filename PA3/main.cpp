#include "kdtreee.hpp"
#include <iostream>
using namespace std;

int main()
{
    
    vector<pair<tuple<int, int>, double>> v{};
    KDTree<tuple<int, int>, double> tree{v};
    // auto &x = const_cast<tuple<int, int, int> &>(tree.root->key());
    // x = {1, 2, 3};
    // auto &y = tree.root->key();
    // cout << get<0>(y) << ' ' << get<1>(y) << ' ' << get<2>(y) << endl;
    tree.insert({10, 8}, 3.5);
    tree.insert({12, 3}, 4);
    tree.insert({6, 7}, 3.8);
    tree.insert({32, 11}, 2);
    tree.insert({5, 12}, 4.8);
    tree.insert({21, 13}, 3.4);
    tree.insert({3, 10}, 5);
    tree.insert({4, 27}, 4.9);
    tree.insert({1, 1}, 3.9);
    tree.insert({13, 9}, 2.7);
    tree.erase({1,1});
    for (auto &item : tree)
    {
        cout << item.second << "\n";
    }
    return 0;
}