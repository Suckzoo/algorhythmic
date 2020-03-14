#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef long long llint;

struct threadknot {
    int x;
    int l;
    bool operator < (const threadknot &k) const {
        return x < k.x;
    }
};
threadknot a[100010];
int n;

bool is_possible_distance(int p) {
    int last_knot = a[0].x;
    debug("0th last knot was at: %d", last_knot);
    int i;
    for(i=1;i<n;i++) {
        int e = a[i].x + a[i].l;
        if (last_knot + p > e) {
            debug("sorry, %d was not possible.", p);
            return false;
        }
        else if (last_knot + p > a[i].x) last_knot = last_knot + p;
        else last_knot = a[i].x;
        debug("%dth last knot was at: %d", i, last_knot);
    }
    return true;
}

int main() {
    scanf("%d",&n);
    int i;
    for(i=0;i<n;i++) {
        scanf("%d%d",&a[i].x,&a[i].l);
    }
    sort(a, a+n);
    int s = 0;
    int e = a[n-1].x + a[n-1].l;
    int p;
    int max_dist = 0;
    while (s <= e) {
        p = s + (e-s)/2;
        if (is_possible_distance(p)) {
            debug("p at %d is possible.", p);
            max_dist = p;
            s = p+1;
        } else {
            e = p-1;
        }
    }
    printf("%d\n", max_dist);
    return 0;
}
