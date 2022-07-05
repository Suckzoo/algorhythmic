#include <cstdio>
#include <iostream>
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

#define endl "\n"

typedef pair<int, int> pii;
typedef long long llint;

int n, m;
struct Node {
    int p;
    llint c, l;
    int cover_s, cover_e;
    llint children_sum;
    vector<int> children;
    Node(int _p, llint _c, llint _l): p(_p), c(_c), l(_l) {
        cover_s = cover_e = -1;
        children_sum = 0;
    }
};
struct SegTreeNode {
    int s, e; // means interval
    llint max_cost;
    int max_cost_idx;
    int count;
    int left, right; // means node index
    SegTreeNode(int _s, int _e): s(_s), e(_e) {
        max_cost = -1;
        max_cost_idx = -1;
        left = right = -1;
        count = 0;
    }
};

struct SegTree {
    vector<SegTreeNode> tree;
    int num_node;
    SegTree(int n): num_node(0) {
        createTree(1, n);
    }
    int createTree(int s, int e) {
        debug("createTree: (%d, %d)", s, e);
        if (s > e) return -1;
        tree.push_back(SegTreeNode(s, e));
        int node_idx = tree.size() - 1;
        if (s == e) return node_idx;
        int p = s + (e - s) / 2;
        int l = createTree(s, p);
        int r = createTree(p+1, e);
        tree[node_idx].left = l;
        tree[node_idx].right = r;
        return node_idx;
    }
    tuple<llint, int, int> resync(const SegTreeNode &node) {
        SegTreeNode left = tree[node.left], right = tree[node.right];
        if (left.count == 0) {
            return {right.max_cost, right.max_cost_idx, right.count};
        } else if(right.count == 0) {
            return {left.max_cost, left.max_cost_idx, left.count};
        }
        if (left.max_cost > right.max_cost) {
            return {left.max_cost, left.max_cost_idx, left.count + right.count};
        }
        return {right.max_cost, right.max_cost_idx, left.count + right.count};
    }
    pair<llint, int> getMaxOfInterval(int i, int s, int e) {
        int p = tree[i].s + (tree[i].e - tree[i].s) / 2;
        if (tree[i].s == s && tree[i].e == e) {
            return {tree[i].max_cost, tree[i].max_cost_idx};
        }
        if (p < s) {
            return getMaxOfInterval(tree[i].right, s, e);
        }
        if (e <= p) {
            return getMaxOfInterval(tree[i].left, s, e);
        }
        auto left = getMaxOfInterval(tree[i].left, s, p);
        auto right = getMaxOfInterval(tree[i].right, p+1, e);
        if (left.first > right.first) {
            return left;
        }
        return right;
    }
    int count(int i, int s, int e) {
        debug("i: %d, s: %d, e: %d", i, s, e);
        debug("i covers (%d, %d)", tree[i].s, tree[i].e);
        int p = tree[i].s + (tree[i].e - tree[i].s) / 2;
        if (tree[i].s == s && tree[i].e == e) {
            return tree[i].count;
        }
        if (p < s) {
            return count(tree[i].right, s, e);
        }
        if (e <= p) {
            return count(tree[i].left, s, e);
        }
        int left = count(tree[i].left, s, p);
        int right = count(tree[i].right, p+1, e);
        return left + right;
    }
    void updateAt(int i, int loc, llint v) {
        if (tree[i].s == loc && tree[i].e == loc) {
            tree[i].max_cost = v;
            tree[i].max_cost_idx = loc;
            tree[i].count = v == -1 ? 0 : 1;
            return;
        }
        int p = tree[i].s + (tree[i].e - tree[i].s) / 2;
        if (loc <= p) {
            updateAt(tree[i].left, loc, v);
        } else {
            updateAt(tree[i].right, loc, v);
        }
        llint max_v;
        int max_i, count;
        tie(max_v, max_i, count) = resync(tree[i]);
        tree[i].max_cost = max_v;
        tree[i].max_cost_idx = max_i;
        tree[i].count = count;
    }
    void update(int loc, llint v) {
        updateAt(0, loc, v);
    }
    int insert(llint v) {
        num_node++;
        update(num_node, v);
        return num_node;
    }
    void remove(int loc) {
        updateAt(0, loc, -1);
    }
};

SegTree *segtree = nullptr;
vector<Node> a;

void input() {
    a.push_back(Node(0, 0, 0));
    cin >> n >> m;
    for(int i=1;i<=n;i++) {
        int p;
        llint c, l;
        cin >> p >> c >> l;
        a.push_back(Node(p, c, l));
        if (p != 0) {
            a[p].children.push_back(i);
        }
    }
    segtree = new SegTree(n);
}

llint ans = 0;
llint dfs(int idx) { // returns interval end and children sum
    debug("Ninja %d", idx);
    int idx_min = segtree->num_node + 1;
    int l = a[idx].children.size();
    llint children_sum = 0;
    for(int i=0;i<l;i++) {
        auto sum = dfs(a[idx].children[i]);
        children_sum += sum;
    }
    children_sum += a[idx].c;
    debug("Inserting Ninja %d...", idx);
    int idx_max = segtree->insert(a[idx].c);
    debug("Ninja %d: (%d, %d)", idx, idx_min, idx_max);
    a[idx].cover_s = idx_min;
    a[idx].cover_e = idx_max;
    debug("Counting...");
    int nn = segtree->count(0, idx_min, idx_max);
    debug("nn: %d", nn);
    while(children_sum > m) {
        auto t = segtree->getMaxOfInterval(0, idx_min, idx_max);
        children_sum -= t.first;
        segtree->remove(t.second);
        nn--;
    }
    a[idx].children_sum = children_sum;
    if (ans < a[idx].l * nn) {
        ans = a[idx].l * nn;
    }
    return a[idx].children_sum;
}

void solve() {
    dfs(1);
    cout << ans << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    input();
    solve();
    return 0;
}
