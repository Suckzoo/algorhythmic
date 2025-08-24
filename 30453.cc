#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <map>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

#define endl "\n"

typedef pair<int, int> pii;
typedef long long llint;

int n;
pii a[3030];
unordered_map<llint, int> symmetry_map;

llint encode(llint x, llint y) {
    return (x + 2000000000LL) * 4000000000LL + y + 2000000000LL;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> n;
    for (int i=0;i<n;i++) {
        cin >> a[i].first >> a[i].second;
    }
    int size_max = 0;
    for (int i=0;i<n;i++) {
        for (int j=i;j<n;j++) {
            llint p = encode(a[i].first + a[j].first, a[i].second + a[j].second);
            if (symmetry_map.find(p) == symmetry_map.end()) {
                symmetry_map[p] = 0;
            }
            if (i == j) symmetry_map[p]++;
            else symmetry_map[p]+=2;
            if(symmetry_map[p] > size_max) {
                size_max = symmetry_map[p];
            }
        }
    }
    cout << size_max << endl;
    return 0;
}
