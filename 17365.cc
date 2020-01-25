#include <cstdio>
#include <cstring>
#include <memory.h>
#include <vector>
#include <algorithm>
#include <queue>
#define THAT_NUMBER 1000000007ll
using namespace std;
typedef long long llint;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", \
__FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

int T;
int n;
char x[200020];
char word[330];
llint dy[200020];

int cvt(char x) {
    // return x - 'a'; //blow up
    return x;
}
struct trie_node {
    int parent;
    int rep;
    int fail;
    llint weight;
    bool finality;
    int portal[128];
    void init_portal() {
        memset(portal, -1, sizeof(portal));
    }
    trie_node() {
        parent = rep = -1;
        finality = false;
        fail = 0;
        weight = 0ll;
        init_portal();
    }
    trie_node(int parent, int rep, bool finality): parent(parent), rep(rep), finality(finality) {
        fail = 0;
        weight = 0ll;
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
        trie[current].weight++;
    }
}

struct dpseed {
    int trie_idx;
    int x_idx;
    int orig;
};
queue <dpseed> dpque;

int main() {
    trie_node root;
    trie.push_back(root);
    scanf("%d",&n);
    int i;
    for(i=0;i<n;i++) {
        scanf("%s",word);
        trie_insert(word, strlen(word));
    }
    scanf("%s",x+1);
    dy[0] = 1;
    int L = strlen(x+1);
    int idx;
    if ((idx = trie[0].portal[cvt(x[1])]) != -1) {
        dpseed head, tail;
        head.trie_idx = idx;
        head.x_idx = 1;
        head.orig = 0;
        dpque.push(head);
        int last_eval = 0;
        while(!dpque.empty()) {
            head = dpque.front();
            debug("orig: %d, trie_idx: %d, x_idx: %d", head.orig, head.trie_idx, head.x_idx);
            dpque.pop();
            if (last_eval != head.x_idx && last_eval < L) {
                last_eval = head.x_idx;
                char next_letter = x[head.x_idx + 1];
                if ((idx = trie[0].portal[cvt(next_letter)]) != -1) {
                    tail.trie_idx = idx;
                    tail.orig = head.x_idx;
                    tail.x_idx = head.x_idx + 1;
                    dpque.push(tail);
                }
            }
            llint incr = (dy[head.orig] * trie[head.trie_idx].weight);
            debug("Incrementing dy[%d] %lld -> %lld (+%lld)", head.x_idx, dy[head.x_idx], dy[head.x_idx] + incr, incr);
            dy[head.x_idx] = (dy[head.x_idx] + incr) % THAT_NUMBER;
            if (head.x_idx < L) {
                char next_letter = x[head.x_idx + 1];
                if ((idx = trie[head.trie_idx].portal[cvt(next_letter)]) != -1) {
                    tail.trie_idx = idx;
                    tail.orig = head.orig;
                    tail.x_idx = head.x_idx + 1;
                    debug("pushing orig: %d, trie_idx: %d, x_idx: %d", tail.orig, tail.trie_idx, tail.x_idx);
                    dpque.push(tail);
                }
            }
        }
    }
    printf("%lld\n",dy[L]);
    return 0;
}