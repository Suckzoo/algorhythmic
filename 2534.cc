#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

#define endl "\n"

typedef pair<int, int> pii;
typedef long long llint;
int n, k, p;
vector<int> e[300030], e_rev[300030];
int indeg[300030], indeg_rev[300030];
llint max_assign[300030], min_assign[300030], diff[300030];
const int MAGIC_NUMBER = 1000000007;

void input() {
    cin >> n >> k >> p;
    for(int i=0;i<p;i++) {
        int x, y;
        cin >> x >> y;
        e[x].push_back(y);
        e_rev[y].push_back(x);
        indeg[y]++;
        indeg_rev[x]++;
    }
}

void get_max() {
    priority_queue<int, vector<int>, greater<int>> que;
    int i;
    for(i=0;i<k;i++) {
        if (!indeg_rev[i]) que.push(i);
    }
    llint assign = n - k;
    while(!que.empty()) {
        int x = que.top();
        que.pop();
        max_assign[x] = assign++;
        for(int y: e_rev[x]) {
            indeg_rev[y]--;
            if (!indeg_rev[y]) que.push(y);
        }
    }
#ifdef DEBUG
    for(int i=k-1;i>=0;i--) {
        debug("max_assign[%d] = %lld", i, max_assign[i]);
    }
#endif
}

void get_min() {
    priority_queue<int, vector<int>, greater<int>> que;
    for(int i=0;i<k;i++) {
        if (!indeg[i]) que.push(i);
    }
    llint assign = k-1;
    while(!que.empty()) {
        int x = que.top();
        que.pop();
        min_assign[x] = assign--;
        for(int y: e[x]) {
            indeg[y]--;
            if (!indeg[y]) que.push(y);
        }
    }
#ifdef DEBUG
    for(int i=k-1;i>=0;i--) {
        debug("min_assign[%d] = %lld", i, min_assign[i]);
    }
#endif
}

void solve() {
    get_max();
    get_min();
    for(int i=0;i<k;i++) {
        diff[i] = max_assign[i] - min_assign[i];
        if (diff[i] < 0) {
            diff[i] += n;
            max_assign[i+1]--;
        }
    }
    llint diff_num = 0;
    for(int i=k-1;i>=0;i--) {
        diff_num *= n;
        diff_num += diff[i];
        diff_num %= MAGIC_NUMBER;
    }
    cout << diff_num << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    input();
    solve();
    return 0;
}
