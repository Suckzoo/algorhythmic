#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <utility>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef long long llint;

struct vec2 {
    int x;
    int y;
    const bool parallel(const vec2 &other) const {
        return x * other.y == y * other.x;
    }
    const int normsq() const {
        return x * x + y * y;
    }
    vec2 operator - (const vec2 &other) const {
        return vec2{x - other.x, y - other.y};
    }
};
struct IndexedVec2 : vec2 {
    int i;
};

struct Point {
    int x;
    int y;
    int i;
    vec2 operator - (const Point &p) const {
        return vec2{x - p.x, y - p.y};
    }
};
Point points[2010];
IndexedVec2 vectors[2010], vectorsSortedInYX[2010];
stack<IndexedVec2> s, replay;
int check[2010];
int n;

int ccw(const vec2 v1, const vec2 v2) {
    int cross = v1.x * v2.y - v1.y * v2.x;
    if (cross > 0) return 1;
    if (cross < 0) return -1;
    return 0;
}

bool atanAscWithNormDesc(const IndexedVec2 &v1, const IndexedVec2 &v2) {
    // this defines p1 < p2
    int c = ccw(v1, v2);
    return c > 0 || (!c && v1.normsq() > v2.normsq());
}
bool yxDesc(const IndexedVec2 &p1, const IndexedVec2 &p2) {
    return p1.y > p2.y || (p1.y == p2.y && p1.x > p2.x);
}

void reset() {
    fill(begin(check), end(check), 0);
#ifdef DEBUG
    assert(s.empty());
    assert(replay.empty());
#endif
}

void dumpvectors(IndexedVec2 vec[]) {
#ifdef DEBUG
    debug("====DUMP VEC BEGINS====");
    for(int i=0;i<n;i++) {
        debug("DBGVEC: vec[%d]: (%d, %d), original index %d", i, vec[i].x, vec[i].y, vec[i].i);
    }
    debug("====DUMP VEC ENDS====");
#endif
}
void dumpstack() {
#ifdef DEBUG
    debug("====DUMP STACK BEGINS====");
    stack <IndexedVec2> s_copy;
    while (!s.empty()) {
        auto top = s.top();
        s.pop();
        debug("(%d, %d)", top.x, top.y);
        s_copy.push(top);
    }
    while (!s_copy.empty()) {
        s.push(s_copy.top());
        s_copy.pop();
    }
    debug("====DUMP STACK ENDS====");
#endif
}

void solve() {
    debug();
    debug("SOLVE BEGINS");
    cin >> n;
    int idxOfMinY = 0, idxOfMaxY = 0;
    for(int i=0;i<n;i++) {
        cin >> points[i].x >> points[i].y;
        points[i].i = i;
        if (points[i].y < points[idxOfMinY].y || (points[i].y == points[idxOfMinY].y && points[i].x < points[idxOfMinY].x)) {
            idxOfMinY = i;
        }
        if (points[i].y > points[idxOfMaxY].y || (points[i].y == points[idxOfMaxY].y && points[i].x > points[idxOfMaxY].x)) {
            idxOfMaxY = i;
        }
    }
    for(int i=0;i<n;i++) {
        auto v = points[i] - points[idxOfMinY];
        vectors[i] = IndexedVec2{
            {
                .x = v.x,
                .y = v.y,
            },
            .i = i
        };
    }
    swap(vectors[0], vectors[idxOfMinY]);
    debug("Before atanAscWithNormDesc Sort");
    dumpvectors(vectors);
    sort(vectors + 1, vectors + n, atanAscWithNormDesc);
    debug("After atanAscWithNormDesc Sort");
    dumpvectors(vectors);
    s.push(vectors[0]);
    s.push(vectors[1]);
    for(int i=2;i<n;i++) {
        if (vectors[i].parallel(vectors[i-1])) {
            debug("parallel detected: (%d, %d) // (%d, %d)", vectors[i].x, vectors[i].y, vectors[i-1].x, vectors[i-1].y);
            continue;
        }
        while(s.size() >= 2) {
            // fetch two top idxvecs
            auto p2 = s.top();
            s.pop();
            auto p1 = s.top();
            s.pop();
            // ccw check
            auto v21 = p2 - p1;
            auto v31 = vectors[i] - p1;
            if (ccw(v21, v31) >= 0) {
                s.push(p1);
                s.push(p2);
                break;
            } else {
                s.push(p1);
            }
        }
        s.push(vectors[i]);
        dumpstack();
    }
    debug("%lu", s.size());
    copy(vectors, vectors + n, vectorsSortedInYX);
    sort(vectorsSortedInYX, vectorsSortedInYX + n, yxDesc);
    debug("Vectors Sorted in YX Desc");
    dumpvectors(vectorsSortedInYX);
    bool ascPath = false;
    bool initDescPath = false;
    int k = 0;
    while(!s.empty()) {
        auto top = s.top();
        s.pop();
        debug("IdxVec2 Dump: [%d](%d, %d)", top.i, top.x, top.y);
        if (ascPath) {
            for(;k<n;k++) {
                if (check[vectorsSortedInYX[k].i]) {
                    continue;
                }
                if (vectorsSortedInYX[k].y >= top.y && vectorsSortedInYX[k].i != top.i) {
                    debug("INCLUDING NON-CVX HULL PT: [%d](%d, %d)", vectorsSortedInYX[k].i, vectorsSortedInYX[k].x, vectorsSortedInYX[k].y);
                    check[vectorsSortedInYX[k].i] = 1;
                    replay.push(vectorsSortedInYX[k]);
                } else {
                    break;
                }
            }
            debug("ASC CVX HULL PT: [%d](%d, %d)", top.i, top.x, top.y);
        } else {
            if (!initDescPath) {
                for(int i=n-1;i>=0;i--) {
                    if (vectors[i].parallel(top) && vectors[i].i != top.i) {
                        debug("INCLUDING DESC HULL ON-LINE PT: [%d](%d, %d)", vectors[i].i, vectors[i].x, vectors[i].y);
                        check[vectors[i].i] = 1;
                        replay.push(vectors[i]);
                    } else {
                        break;
                    }
                }
                initDescPath = true;
            }
            debug("DESC CVX HULL PT: [%d](%d, %d)", top.i, top.x, top.y);
        }
        if (top.i == idxOfMaxY) {
            //descending path over, ascending path starts
            debug("ASC MODE ON");
            ascPath = true;
        }
        check[top.i] = 1;
        replay.push(top);
    }
    debug("SWEEP OVER");
    while(!replay.empty()) {
        debug("REPLAY: [%d](%d, %d)", replay.top().i, replay.top().x, replay.top().y);
        cout << replay.top().i << " ";
        replay.pop();
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int T;
    cin >> T;
    while(T--) {
        solve();
        reset();
    }
    return 0;
}
