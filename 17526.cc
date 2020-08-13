#include <cstdio>
#include <cstring>
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

typedef pair<int, int> pii;
typedef long long llint;

struct dpline {
    llint m;
    llint b;
    llint x;
};

llint ceil_fact(llint a, llint b){
    if (a%b) return a/b + 1;
    return a/b;
}

llint xross(dpline opt, dpline newline) {
    return ceil_fact((newline.b - opt.b), (opt.m - newline.m));
}

int n;

dpline hull[100010];
int top;

llint dist[100010];
llint sum[100010];
llint dy[100010];

struct ships {
    llint pace;
    llint prep;
};
ships a[100010];

int find_riding_line(int target_pace) {
    int s = 0;
    int e = top - 1;
    int ans = top - 1;
    if (hull[top-1].x <= target_pace) {
        return top - 1;
    }
    while(s <= e) {
        int p = s + (e - s) / 2;
        dpline middle_lane = hull[p];
        dpline next_lane = hull[p+1];
        if (middle_lane.x <= target_pace && target_pace < next_lane.x) {
            ans = p;
            break;
        } else if (target_pace < middle_lane.x) {
            e = p - 1;
        } else {
            s = p + 1;
        }
    };
    return ans;
}

int main() {
    scanf("%d",&n);
    int i;
    for(i=n-1;i>0;i--) {
        scanf("%lld",&dist[i]);
    }
    for(i=1;i<n;i++) {
        sum[i] = sum[i-1] + dist[i];
    }
    for(i=n-1;i>0;i--) {
        scanf("%lld%lld", &a[i].prep, &a[i].pace);
    }
    dpline init_line;
    init_line.m = 0;
    init_line.b = 0;
    init_line.x = 0;
    hull[0] = init_line;
    top = 1;
    for(i=1;i<n;i++) {
        // 1. find riding line;
        debug("finding riding line...");
        int riding_line_index = find_riding_line(a[i].pace);
        dpline riding_line = hull[riding_line_index];
        debug("riding line for pace %lld is %lldx + %lld", a[i].pace, riding_line.m, riding_line.b);

        // 2. calc dy[i];
        llint c, mx, b;
        c = a[i].prep + a[i].pace * sum[i];
        mx = riding_line.m * a[i].pace;
        b = riding_line.b;
        dy[i] = c + mx + b;
        
        // 3. insert new line to hull;
        dpline new_line;
        new_line.m = -sum[i];
        new_line.b = dy[i];
        new_line.x = -1;
        debug("new line is %lldx + %lld", new_line.m, new_line.b);
        while(top) {
            debug("xross with %d", top - 1);
            dpline end_line = hull[top-1];
            llint xross_point = xross(end_line, new_line);
            debug("xrosspoint is with %lld", xross_point);
            if (xross_point <= end_line.x) {
                top--;
            } else {
                new_line.x = xross_point;
                break;
            }
        }
        hull[top++] = new_line;
    }
    printf("%lld\n",dy[n-1]);
    return 0;
}
