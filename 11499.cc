#include <cstdio>
#include <cstring>
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

#define X(p) ((p).first)
#define Y(p) ((p).second)

typedef long long llint;
typedef pair<llint, llint> pii;
typedef pii vec2d;

vector<pii> a, d;
struct PolygonPoint {
    pii p;
    int isDest;
};
vector<PolygonPoint> polygon;
vector<PolygonPoint> hull;
double ans;
int n,m;

inline int Abs(int x) {
    return x < 0 ? -x : x;
}

inline double norm(const pii &p) {
    double x = (double)X(p);
    double y = (double)Y(p);
    return sqrt(x * x + y * y);
}

pii operator + (const pii &l, const pii &r) {
    return {l.first + r.first, l.second + r.second};
}

pii operator - (const pii &l, const pii &r) {
    return {l.first - r.first, l.second - r.second};
}

pii operator * (const pii &l, const int &r) {
    return {l.first * r, l.second * r};
}

pii operator / (const pii &l, const int &r) {
    return {l.first / r, l.second / r};
}

pii translate(vector<pii> &poly, pii pt) {
    pii prev = poly[pt.first];
    pii next = poly[(pt.first + 1) % poly.size()];
    pii dir = next - prev;
    if (dir.first) dir = dir / Abs(dir.first);
    else dir = dir / Abs(dir.second);
    return prev + (dir * pt.second);
}

double atanpii(const pii &p) {
    return atan2(Y(p), X(p));
}

bool atan2cmp(const pii &x, const pii &y) {
    return atan2(Y(x), X(x)) < atan2(Y(y), X(y));
}
bool deltacmp(const pii &x, const pii &y) {
    return x.first < y.first || (x.first == y.first && x.second < y.second);
}

int ccw(vec2d v1, vec2d v2) {
    llint cross = X(v1) * Y(v2) - Y(v1) * X(v2);
    if (cross < 0) return -1;
    else if (cross > 0) return 1;
    return 0;
}

int main() {
    int T;
    scanf("%d",&T);
    while(T--) {
        scanf("%d",&n);
        pii pt, p0;
        for(int i=0;i<n;i++) {
            scanf("%lld%lld",&X(pt),&Y(pt));
            if (!i) {
                p0 = pt;
            }
            a.push_back(pt - p0);
        }
        scanf("%d",&m);
        pii px;
        for(int i=0;i<m;i++) {
            scanf("%lld%lld",&X(px),&Y(px));
            d.push_back(px);
        }
        sort(d.begin(), d.end(), deltacmp);
        int dindex = 0;
        for(int i=0;i<n;i++) {
            PolygonPoint apt;
            apt.p = a[i];
            apt.isDest = 0;
            polygon.push_back(apt);
            while(d[dindex].first == i) {
                PolygonPoint dpt;
                dpt.p = translate(a, d[dindex]);
                dpt.isDest = 1;
                polygon.push_back(dpt);
                dindex++;
            }
        }
        n = polygon.size();

        double hullLength = 0;
        hull.push_back(polygon[0]);
        for(int i = n-1; i > 0; i--) {
            while(hull.size() > 1) {
                int top = hull.size() - 1;
                PolygonPoint p1, p2;
                p1 = hull[top - 1];
                p2 = hull[top];
                int ccwVal = ccw(p2.p - p1.p, polygon[i].p - p1.p);
                if (ccwVal > 0) {
                    break;
                } else {
                    hullLength -= norm(p2.p - p1.p);
                    hull.pop_back();
                }
            }
            hull.push_back(polygon[i]);
            int top = hull.size() - 1;
            PolygonPoint p1, p2;
            p1 = hull[top - 1];
            p2 = hull[top];
            hullLength += norm(p2.p - p1.p);
            if (polygon[i].isDest) {
                ans += hullLength;
            }
        }

        printf("%.1lf\n", ans);
        a.clear();
        d.clear();
        polygon.clear();
        hull.clear();
        ans = 0;
    }
    return 0;
}
