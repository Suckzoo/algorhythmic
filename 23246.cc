#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
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
struct climber {
    int back;
    int r1;
    int r2;
    int r3;
    int prod() const  {
        return r1 * r2 * r3;
    }
    int sum() const {
        return r1 + r2 + r3;
    }
    bool operator < (const climber &x) const {
        return prod() < x.prod()
            || (prod() == x.prod() && sum() < x.sum())
            || (prod() == x.prod() && sum() == x.sum() && back < x.back);
    }
};
climber a[110];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> n;
    for(int i=0;i<n;i++) {
        cin >> a[i].back >> a[i].r1 >> a[i].r2 >> a[i].r3;
    }
    sort(a, a+n);
    cout << a[0].back << " " << a[1].back << " " << a[2].back << endl;
    return 0;
}
