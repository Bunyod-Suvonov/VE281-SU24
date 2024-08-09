#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long int lli;

struct Point{
    lli x;
    lli y;
};

// function to find the point with the lowest y value
Point findP0(const vector<Point> &coordinates){
    lli minY = coordinates[0].y;
    Point minCoordinate = coordinates[0];
    for(auto &coordinate:coordinates){
        if(coordinate.y < minY || (coordinate.y == minY && coordinate.x < minCoordinate.x)){
            minY = coordinate.y;
            minCoordinate = coordinate;
        }
    }
    return minCoordinate;
}

long double ccw(const Point &a, const Point &b, const Point &c){
    return ((double) b.x-(double) a.x) * ((double) c.y-(double) a.y) - ((double) b.y-(double) a.y) * ((double) c.x-(double) a.x);
}

// custom compare function
struct CompareLess {
    Point P0;
    bool operator()(const Point &P1, const Point &P2) const {
        long double ccw_res = ccw(P0, P1, P2);
        if(ccw_res > 0) return true;
        else if(ccw_res==0){
            return ((P1.y-P0.y)<(P2.y-P0.y) || (((P2.y-P0.y) == (P1.y-P0.y)) && (P1.x-P0.x)<(P2.x-P0.x)));
        }
        return false;
    }
};


int main(){
    // initialize, get input and store
    int N;
    cin >> N;
    if(N<=0) return 0;
    // set of points
    vector<Point> X;
    // stack
    vector<Point> S;
    for(int i=0; i<N; i++){
        Point a;
        cin >> a.x >> a.y;
        X.push_back(a);
    }

    Point P0 = findP0(X);
    CompareLess comp;
    comp.P0 = P0;

    // remove P0 from X
    for(auto it=X.begin(); it!=X.end(); it++){
        if(it->x == P0.x && it->y == P0.y){
            std::iter_swap(it, X.end()-1);
            X.pop_back();
            break;
        }
    }

    sort(X.begin(), X.end(), comp);

    S.push_back(P0);
    for(auto it=X.begin(); it!=X.end(); it++){
        while(S.size() > 1 && ccw(S[S.size()-2], S[S.size()-1], *it) <= 0){
            S.pop_back();
        }
        S.push_back(*it);
    }

    if(S.size()==2 && S[0].x == S[1].x && S[0].y == S[1].y){
        cout << S[0].x << ' ' << S[0].y << endl;
        return 0;
    }

    // print elements of stack
    for(auto it=S.begin(); it!=S.end(); it++){
        cout << it->x << ' ' << it->y << endl;
    }

    return 0;
}