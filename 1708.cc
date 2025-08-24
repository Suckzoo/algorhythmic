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

#define EPS 1e-10
#define X(p) ((p).first)
#define Y(p) ((p).second)
#define endl "\n"

typedef long long llint;
typedef pair<llint, llint> pii;

int n;
pii a[100100];

stack<pii> hull;

pii operator - (const pii p1, const pii p2) {
    return make_pair(X(p1)-X(p2), Y(p1)-Y(p2));
}

bool cmp(const pii p1, const pii p2) {
    return Y(p1) < Y(p2) || (Y(p1) == Y(p2) && X(p1) < X(p2));
}

double norm(const pii v) {
    return X(v) * X(v) + Y(v) * Y(v);
}

function<bool(pii, pii)> cmpata(pii ref) {
    return [ref](const pii p1, const pii p2) {
        auto v1 = p1 - ref;
        auto v2 = p2 - ref;
        auto ata1 = atan2(Y(v1), X(v1));
        auto ata2 = atan2(Y(v2), X(v2));
        if (fabs(ata1 - ata2) < EPS) {
            return norm(v1) < norm(v2);
        }
        return ata1 < ata2;
    };
}

int ccw(pii ref, pii p2, pii p3) {
    auto v1 = p2 - ref;
    auto v2 = p3 - ref;
    auto prod = X(v1)*Y(v2) - X(v2)*Y(v1);
    if (prod < 0) return -1;
    else if (prod > 0) return 1;
    return 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> X(a[i]) >> Y(a[i]);
    }
    sort(a, a+n, cmp);
    sort(a+1, a+n, cmpata(a[0]));
    hull.push(a[0]);
    hull.push(a[1]);
    for (int i = 2; i < n; i++) {
        auto p = a[i];
        while(hull.size() > 1) {
            auto curr_ref = hull.top();
            hull.pop();
            auto prev_ref = hull.top();
            auto c = ccw(prev_ref, curr_ref, p);
            debug("prev_ref: (%lld, %lld), curr_ref: (%lld, %lld), p: (%lld, %lld), c: %d",
            X(prev_ref), Y(prev_ref), X(curr_ref), Y(curr_ref), X(p), Y(p), c);
            if (c == 0) {
                hull.push(p);
                break;
            } else if (c > 0) {
                hull.push(curr_ref);
                hull.push(p);
                break;
            }
        }
    }
    cout << hull.size() << endl;
    #ifdef DEBUG
    while (!hull.empty()) {
        auto p = hull.top();
        hull.pop();
        cout << "(" << X(p) << ", " << Y(p) << ")" << endl;
    }
    #endif
    return 0;
}
