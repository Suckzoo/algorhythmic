#include <cstdio>

int n;
long long a[100010];
long long b[100010];
long long dy[100010];

struct dpline {
    long long m;
    long long b;
    long long x;
};
dpline hull[100010];
int top = 0;

long long ceil_fact(long long a, long long b){
    if (a%b) return a/b + 1;
    return a/b;
}

long long xross(dpline opt, dpline newline) {
    return ceil_fact((newline.b - opt.b), (opt.m - newline.m));
}

int main() {
    scanf("%d",&n);
    int i;
    for(i=0;i<n;i++) {
        scanf("%lld",&a[i]);
    }
    for(i=0;i<n;i++) {
        scanf("%lld",&b[i]);
    }

    dpline basis;
    basis.m = b[0];
    basis.b = 0;
    basis.x = 0;
    hull[top++] = basis;

    int riding_line_idx = 0;

    for(i=1;i<n;i++) {
        //todo: riding_line_idx adjust
        while(riding_line_idx + 1 < top) {
            dpline cand = hull[riding_line_idx];
            dpline next = hull[riding_line_idx + 1];
            if (cand.x <= a[i] && a[i] < next.x) break;
            riding_line_idx++;
        }
        dpline opt = hull[riding_line_idx];
        dy[i] = opt.m * a[i] + opt.b;
        dpline newline;
        newline.m = b[i];
        newline.b = dy[i];
        newline.x = -1; //unknown
        while(top > 0) {
            dpline hullline = hull[top-1];
            long long xx = xross(hullline, newline);
            if (xx > hullline.x) {
                newline.x = xx;
                break;
            }
            top--;
        }
        hull[top++] = newline;
    }
    printf("%lld\n",dy[n-1]);

    return 0;
}