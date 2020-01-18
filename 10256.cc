#include <cstdio>
#include <cstring>
#include <memory.h>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

int T;
int n, m;
char x[1000010];
char marker[110];

int cvt(char x) {
    switch(x) {
    case 'A':
        return 0;
    case 'G':
        return 1;
    case 'T':
        return 2;
    case 'C':
        return 3;
    }
    return -1; //blow up
}
struct trie_node {
    int parent;
    int rep;
    int fail;
    bool finality;
    int portal[4];
    void init_portal() {
        memset(portal, -1, sizeof(portal));
    }
    trie_node() {
        parent = rep = -1;
        finality = false;
        fail = 0;
        init_portal();
    }
    trie_node(int parent, int rep, bool finality): parent(parent), rep(rep), finality(finality) {
        fail = 0;
        init_portal();
    }
};

vector<trie_node> trie;

void trie_insert(char p[], int l) {
    int current = 0;
    for(int i=0;i<l;i++) {
        int c = cvt(p[i]);
        if (trie[current].portal[c] != -1) {
            current = trie[current].portal[c];
            if (i == l-1) {
                trie[current].finality = true;
            }
        } else {
            trie_node new_node(current, c, (i==l-1));
            trie.push_back(new_node);
            int idx = trie.size() - 1;
            trie[current].portal[c] = idx;
            current = idx;
        }
    }
}

struct last_fail {
    int curr;
    int fail;
};
queue<last_fail> que;

void construct_fail_link() {
    for(int i=0;i<4;i++) {
        last_fail f;
        if (trie[0].portal[i] != -1) {
            f.curr = trie[0].portal[i];
            f.fail = 0;
            que.push(f);
        }
    }
    trie[0].fail = 0;
    last_fail head, tail;
    while(!que.empty()) {
        head = que.front();
        que.pop();
        for(int i=0;i<4;i++) {
            if (trie[head.curr].portal[i] != -1) {
                int failidx = head.fail;
                while (trie[failidx].portal[i] == -1 && failidx != 0) {
                    failidx = trie[failidx].fail;
                }
                tail.fail = trie[failidx].portal[i] != -1 ? trie[failidx].portal[i] : 0;
                tail.curr = trie[head.curr].portal[i];
                trie[tail.curr].fail = tail.fail;
                que.push(tail);
            }
        }
    }
}

int main() {
    scanf("%d",&T);
    while(T--) {
        trie_node root;
        trie.push_back(root);
        scanf("%d%d",&n,&m);
        scanf("%s",x);
        scanf("%s",marker);
        trie_insert(marker, m);
        for(int i=0;i<m;i++) {
            for(int j=1;j<m;j++) {
                reverse(marker+i,marker+j+1);
                trie_insert(marker, m);
                reverse(marker+i,marker+j+1);
            }
        }
        construct_fail_link();
        int curr = 0;
        int cnt = 0;
        for(int i=0;i<n;i++) {
            int c = cvt(x[i]);
            while (trie[curr].portal[c] == -1 && curr != 0) {
                curr = trie[curr].fail;
            }
            if (trie[curr].portal[c] != -1) {
                curr = trie[curr].portal[c];
            }
            if (trie[curr].finality) {
                cnt++;
                curr = trie[curr].fail;
            }
        }
        printf("%d\n",cnt);
        trie.clear();
    }
}