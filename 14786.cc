#include <cstdio>
#include <cmath>
#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", \
__FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

double a,b,c;
double cand[500020];
double fval[500020];

double f(double x) {
    return c - a * x - b * sin(x);
}
int main() {
    scanf("%lf%lf%lf",&a,&b,&c);
    double s;
    double e;
    if (a > b) {
        s = 0;
        e = 1.0;
        while(f(e) >= 0) e *= 2;
    } else {
        // c - ax - bsinx -> -a - bcosx = 0 -> cosx = -a/b -> x = acos(-a/b)
        int i;
        double modal = acos(-a/b);
        double pi = acos(-1);
        cand[0] = 0;
        fval[0] = f(0);
        cand[1] = modal;
        fval[1] = f(modal);
        for(i=2;i<500000;i+=2) {
            cand[i] = pi * (double)(i / 2) - modal;
            fval[i] = f(cand[i]);
            cand[i+1] = pi * (double)(i / 2) + modal;
            fval[i+1] = f(cand[i+1]);
        }
        for(i=0;i<499999;i++) {
            if(fval[i] >= 0 && fval[i+1] < 0) {
                s = cand[i];
                e = cand[i+1];
                break;
            }
        }
    }
    double x;
    while(e - s > 1e-10) {
        x = (s + e) / 2;
        debug("s: %.20lf, e: %.20lf", s, e);
        debug("x: %.20lf, f(x): %.20lf", x, f(x));
        if (f(x) > 0) {
            s = x;
        } else {
            e = x;
        }
    }
    printf("%.20lf\n", x);
    return 0;
}
