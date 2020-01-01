#include <cstdio>

int n;
long long A, B, C;
long long a[1000010];
long long s[1000010];
long long dy[1000010];

struct dpline {
    long long m;
    long long b;
    long long x;
};

dpline hull[1000010];
int top = 0;

long long ceil_fact(long long a, long long b){
    if (a%b) return a/b + 1;
    return a/b;
}

long long xross(dpline opt, dpline newline) {
    return ceil_fact((newline.b - opt.b), (opt.m - newline.m));
}

long long f(long long X) {
    return A * X * X + B * X + C;
}

int main() {
    // d[i] = d[k] + a(s[i] - s[k])^2 + b(s[i] - s[k]) + c
    // d[i] = d[k] + as[i]^2 + as[k]^2 - 2as[i]s[k] + bs[i] - bs[k] + c
    // = d[k] + as[k]^2 - 2as[i]s[k] - bs[k] + as[i]^2 + bs[i] + c
    // = max(-2as[k] * s[i] + d[k] + as[k]^2 - bs[k]) + as[i]^2 + bs[i] + c
    scanf("%d", &n);
    scanf("%lld%lld%lld",&A,&B,&C);
    int i;
    for(i=1;i<=n;i++) {
        scanf("%lld",&a[i]);
        s[i] = s[i-1] + a[i];
    }
    dpline basis;
    basis.m = 0;
    basis.b = 0;
    basis.x = 0;
    hull[top++] = basis;
    int riding_line_idx = 0;
    for(i=1;i<=n;i++) {
        while (riding_line_idx + 1 < top) {
            dpline curr, next;
            curr = hull[riding_line_idx];
            next = hull[riding_line_idx + 1];
            if (curr.x <= s[i] && s[i] < next.x) break;
            riding_line_idx++;
        }
        dpline opt = hull[riding_line_idx];
        dy[i] = opt.m * s[i] + opt.b + f(s[i]);
        dpline newline;
        newline.m = -2 * A * s[i];
        newline.b = dy[i] + A * s[i] * s[i] - B * s[i];
        newline.x = -1; // unknown at the moment;
        while (top > 0) {
            dpline hullline = hull[top - 1];
            long long xx = xross(hullline, newline);
            if (xx > hullline.x) {
                newline.x = xx;
                break;
            }
            top--;
        }
        hull[top++] = newline;
    }
    printf("%lld\n", dy[n]);
}