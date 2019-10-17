#include <cstdio>
#include <cstdlib>
#include <utility>
#include <set>
#include <map>
#define x(p) p.first
#define y(p) p.second
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
using namespace std;

typedef pair<int, int> pii;

struct LazyNode {
    int start;
    int partition;
    int end; // equal or less
    int value;
    int lazy;
    
    LazyNode *left;
    LazyNode *right;

    LazyNode(int s, int e): start(s), end(e), value(0), lazy(0) {
        partition = start + (end - start) / 2;
        left = right = nullptr;
    }

    ~LazyNode() {
        if (left) delete left;
        if (right) delete right;
    }

    LazyNode* getLeft() {
        if (!left) {
            left = new LazyNode(start, partition);
        }
        return left;
    }

    LazyNode* getRight() {
        if (!right) {
            right = new LazyNode(partition + 1, end);
        }
        return right;
    }

    void addLazyValue(int delta) {
        lazy += delta;
        value += delta * (end - start + 1);
    }

    void update(int s, int e, int v) { // add v to a[s]~a[e]
        if (s == start && e == end) {
            addLazyValue(v);
            return;
        }
        LazyNode* l = getLeft();
        l->addLazyValue(lazy);
        LazyNode* r = getRight();
        r->addLazyValue(lazy);
        if (s > partition) {
            r->update(s, e, v);
        } else if(e <= partition) {
            l->update(s, e, v);
        } else {
            l->update(s, partition, v);
            r->update(partition + 1, e, v);
        }
        lazy = 0;
    }

    int get(int s, int e) { // sum of a[s]~a[e]
        if (s == start && e == end) {
            return value;
        }
        if (lazy) {
            getLeft()->addLazyValue(lazy);
            getRight()->addLazyValue(lazy);
            lazy = 0;
        }
        if (s > partition) {
            return getRight()->get(s, e);
        }
        if (e <= partition) {
            return getLeft()->get(s, e);
        }
        int lval = getLeft()->get(s, partition);
        int rval = getRight()->get(partition + 1, e);
        return lval + rval;
    }

    int findMaxUnitSegment() {
        if (start == end) return value;
        if (start > end) return 0;
        if (!left && !right) {
            return lazy + (value / (end - start + 1));
        }
        getLeft()->addLazyValue(lazy);
        int l = getLeft()->findMaxUnitSegment();
        getRight()->addLazyValue(lazy);
        int r = getRight()->findMaxUnitSegment();
        lazy = 0;
        return MAX(l, r);
    }
};

int n;
pii a[100010];
set<int> Xs, Ys;
map<int, int> xmap, ymap;

int main() {
    scanf("%d",&n);
    int i;
    for(i=0;i<n;i++) {
        scanf("%d%d",&x(a[i]),&y(a[i]));
        Xs.insert(x(a[i]));
        Ys.insert(y(a[i]));
    }

    i = 0;
    for(auto x : Xs) {
        xmap[x] = i++;
    }
    i = 0;
    for(auto y : Ys) {
        ymap[y] = i++;
    }
    LazyNode *xroot = new LazyNode(0, xmap.size() - 2);
    LazyNode *yroot = new LazyNode(0, ymap.size() - 2);

    for(i=0;i<n;i++) {
        pii p1 = a[i], p2 = a[(i+1)%n];

        if (x(p1) != x(p2)) {
            int minx = xmap[MIN(x(p1), x(p2))], maxx = xmap[MAX(x(p1), x(p2))];
            xroot->update(minx, maxx - 1, 1);
        } else {
            int miny = ymap[MIN(y(p1), y(p2))], maxy = ymap[MAX(y(p1), y(p2))];
            yroot->update(miny, maxy - 1, 1);
        }
    }

    int h = xroot->findMaxUnitSegment();
    int v = yroot->findMaxUnitSegment();

    printf("%d\n",MAX(h,v));
    return 0;
}