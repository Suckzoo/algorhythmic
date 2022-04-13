#include <cstdio>
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

int n;
int a[1000100];
int check[1000100];
int parent_info[1000100][2];
vector<int> equiv_class[1000100];

int find(int v) {
    if (parent_info[v][0] == -1) return v;
    int parent = find(parent_info[v][0]);
    if (parent != parent_info[v][0]) {
        parent_info[v][0] = parent;
    }
    return parent;
}

void union_set(int u, int v) {
    int u_parent = find(u);
    int v_parent = find(v);
    if (u_parent == v_parent) {
        return;
    }
    if (parent_info[u_parent][1] > parent_info[v_parent][1]) {
        parent_info[v_parent][0] = u_parent;
    } else if (parent_info[u_parent][1] < parent_info[v_parent][1]) {
        parent_info[u_parent][0] = v_parent;
    } else {
        if (u_parent > v_parent) {
            swap(u_parent, v_parent);
        }
        parent_info[v_parent][0] = u_parent;
        parent_info[u_parent][1]++;
    }
}

int main() {
    scanf("%d",&n);
    for (int i=0;i<n;i++) {
        scanf("%d",&a[i]);
        a[i]--;
    }
    for (int i=0;i<n;i++) {
        parent_info[i][0] = -1;
    }
    int min_v = 0;
    int max_v = 0;
    for (int i=0;i<n;i++) {
        if (check[a[i]]) continue;
        check[a[i]] = 1;
        if (a[i] > max_v) max_v = a[i];
        debug("union: (%d, %d)", min_v, a[i]);
        union_set(a[i], min_v);
        if (check[min_v]) {
            for(int j=min_v+1;j<n;j++) {
                if (check[j]) continue;
                min_v = j;
                break;
            }
            if (min_v < max_v) {
                debug("union: (%d, %d)", min_v, max_v);
                union_set(min_v, max_v);
            }
        }
    }
    int cnt = 0;
    for (int i=0;i<n;i++) {
        int p = find(i);
        if (equiv_class[p].empty()) cnt++;
        equiv_class[p].push_back(i);
    }
    printf("%d\n", cnt);
    for (int i=0;i<n;i++) {
        if (!equiv_class[i].size()) continue;
        printf("%lu ", equiv_class[i].size());
        for(int j: equiv_class[i]) {
            printf("%d ", j+1);
        }
        printf("\n");
    }
    return 0;
}
