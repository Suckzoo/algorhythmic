#include <cstdio>
#define abs(x) ((x) < 0 ? -(x) : (x))

typedef long long ll;
int n;
ll a[100010];

ll test(ll interval) {
    ll x = interval;
    ll cost = 0;
    for(int i=1;i<n;i++) {
        cost += abs(x - a[i]);
        x += interval;
    }
    return cost;
}

int main() {
    scanf("%d", &n);
    int i;
    for(i=0;i<n;i++) {
        scanf("%lld",&a[i]);
    }

    ll s = 0ll, e = a[n-1];

    while (e - s >= 3) {
        ll p1 = (2 * s + e) / 3;
        ll cost1 = test(p1);
        ll p2 = (s + 2 * e) / 3;
        ll cost2 = test(p2);

        if (cost1 < cost2) {
            e = p2;
        } else if (cost1 > cost2) {
            s = p1;
        } else {
            s = p1;
            e = p2;
        }
    }
    ll min = test(s++);
    for(;s<=e;s++) {
        ll cand = test(s);
        if (cand < min) min = cand;
    }
    printf("%lld\n", min);
}