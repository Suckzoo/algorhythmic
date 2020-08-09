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

#define X(p) ((p).first)
#define Y(p) ((p).second)
#define INF 1000000001


typedef long long llint;
typedef pair<llint, llint> pii;

int n;
pii a[300030];
vector<llint> ys;

template<typename T, typename U = std::less<>>
class SparseTable {
private:
    vector<vector<T>> table;
public:
    static int log2n(int n) {
        int l;
        for(l = 0; n > 1; l++, n=n>>1);
        return l;
    }

    bool compare(T a, T b) {
        U u;
        return u(a, b);
    }

    T getValue(int L, int R) {
        if (L == R) return table[0][L];
        int j = log2n(R - L + 1);
        int k = R - (1<<j) + 1;
        debug("Querying [%d,%d]", L, R);
        debug("j: %d k: %d table[j][L]: %lld, table[j][k]: %lld", j, k, table[j][L], table[j][k]);
        if (compare(table[j][L], table[j][k])) return table[j][L];
        return table[j][k];
    }

    SparseTable() {}
    SparseTable(vector<T> &a) {
        init(a);
    }
    
    void dump() {
        int i, level=0;
        debug("dump");
        for(auto &vt: table){
            fprintf(stderr, "level %d\n", level);
            for (auto v: vt){
                fprintf(stderr, "%d ", v);
            }
            fprintf(stderr, "\n");
            level++;
        }
    }

    void init(vector<T> &a) {
        int n = a.size();
        int depth = log2n(n) + 1;
        int i,j;
        table.resize(depth);
        for(i=0;i<depth;i++) {
            table[i].resize(n);
        }
        for(i=0;i<n;i++) {
            table[0][i] = a[i];
        }
        for(i=1;i<depth;i++) {
            for(j=0;j<n;j++) {
                int k = j + (1 << (i-1));
                if (k >= n) continue;
                if (compare(table[i-1][j], table[i-1][k])) {
                    table[i][j] = table[i-1][j];
                } else {
                    table[i][j] = table[i-1][k];
                }
            }
        }
    }
};
SparseTable<llint> mini;
SparseTable<llint, std::greater<llint>> maxi;

int find_limit(int start, llint coord_limit) {
    debug("Coord limit is %lld", coord_limit);
    int s = start;
    int e = n - 1;
    int p;
    int ans = s;
    while(s<=e) {
        p = s + (e-s)/2;
        debug("%d %d %d", s, e, p);
        if (X(a[p]) <= coord_limit) {
            ans = p;
            s = p+1;
        } else {
            e = p - 1;
        }
    }
    debug("The limit for coord_limit %lld was %lld", coord_limit, X(a[ans]));
    return ans;
}

bool ycanal_with_d(llint p) {
    debug("2d limit is %lld", p);
    int i;
    for(i=0;i<n;i++) {
        debug("Starting at %lld", X(a[i]));
        llint minY1=INF, maxY1=-INF, minY2=INF, maxY2=-INF;
        llint minY, maxY;
        int ycanal_end = find_limit(i, X(a[i]) + p);
        debug("End at %lld", X(a[ycanal_end]));
        if (i > 0) {
            minY1 = mini.getValue(0, i-1); //TODO;
            maxY1 = maxi.getValue(0, i-1); //TODO;
        }
        if (ycanal_end < n-1) {
            minY2 = mini.getValue(ycanal_end + 1, n-1);
            maxY2 = maxi.getValue(ycanal_end + 1, n-1);
        }
        debug("minY1: %lld, maxY1: %lld", minY1, maxY1);
        debug("minY2: %lld, maxY2: %lld", minY2, maxY2);
        minY = minY1 < minY2 ? minY1 : minY2;
        maxY = maxY1 > maxY2 ? maxY1 : maxY2;
        debug("minY: %lld, maxY: %lld", minY, maxY);
        if (maxY - minY <= p) return true;
    }
    return false;
}

llint param(llint dlimit) {
    llint s = 0, e = dlimit, p;
    llint ans = dlimit;
    while(s <= e) {
        p = s + (e - s)/2;
        debug("%lld %lld %lld", s, e, p);
        if (ycanal_with_d(p)) {
            ans = p;
            e = p - 1;
        } else {
            s = p + 1;
        }
    }
    return ans;
}

int main() {
    scanf("%d",&n);
    int i;
    for(i=0;i<n;i++) {
        scanf("%lld%lld",&X(a[i]), &Y(a[i]));
    }
    sort(a, a+n);
    for(i=0;i<n;i++) {
        ys.push_back(Y(a[i]));
    }
    mini.init(ys);
    maxi.init(ys);
    llint ans = param(X(a[n-1]) - X(a[0]));
    printf("%.1f\n", (double)ans / 2.0f);
    // for(i=0;i<n;i++) {
    //     debug("%lld %lld",X(a[i]), Y(a[i]));
    // }
    return 0;
}
