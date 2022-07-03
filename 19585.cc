#include <cstdio>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
#include <map>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

#define endl "\n"

typedef pair<int, int> pii;
typedef long long llint;

int c, n, q;
struct TrieNode {
    string substr;
    bool finish;
    map<string, int> portal;
    TrieNode(string s, bool f): substr(s), finish(f) {}
    int connectionIndex(string frag) {
        return portal.find(frag) != portal.end() ? portal[frag] : -1;
    }
    void dump() {
        #ifdef DEBUG
        debug("%s %d", substr.c_str(), finish);
        #endif
    }
};
struct Trie {
    vector<TrieNode> nodes;
    Trie() {
        nodes.push_back(TrieNode(string("$"), false));
    }
    void dump() {
        #ifdef DEBUG
        for(auto &n: nodes) {
            n.dump();
        }
        #endif
    }
    void insert(const string &x) {
        int idx = 0;
        int l = x.length();
        for(int i=0;i<l;i+=10) {
            string frag;
            if (i + 10 > l) {
                frag = x.substr(i, l-i);
            } else {
                frag = x.substr(i, 10);
            }
            int nextIdx = nodes[idx].connectionIndex(frag);
            if (nextIdx == -1) {
                nextIdx = nodes.size();
                nodes[idx].portal[frag] = nodes.size();
                nodes.push_back(TrieNode(frag, false));
            }
            idx = nextIdx;
        }
        nodes[idx].finish = true;
    }
    bool finishable(int idx) {
        return nodes[idx].finish;
    }
};
Trie color, revName;

void input() {
    cin >> c >> n;
    string s;
    for(int i=0;i<c;i++) {
        cin >> s;
        color.insert(s);
    }
    for(int i=0;i<n;i++) {
        cin >> s;
        reverse(begin(s), end(s));
        revName.insert(s);
    }
    color.dump();
    revName.dump();
    cin >> q;
}

bool rainbowPrefix[1010], rainbowSuffix[1010];
void getRainbowTable(const string &x) {
    fill(begin(rainbowPrefix), end(rainbowPrefix), false);
    fill(begin(rainbowSuffix), end(rainbowSuffix), false);
    int l = x.length();
    int nodeIndex = 0;
    string frag;
    debug("handle name: %s", x.c_str());
    for(int i=0;i<l-1;i++) {
        if (i >= 1000) break;
        frag += x[i];
        int idx = color.nodes[nodeIndex].connectionIndex(frag);
        if (idx != -1 && color.finishable(idx)) {
            debug("Valid prefix length: %d", i+1);
            rainbowPrefix[i+1] = true;
        }
        if (i % 10 == 9) {
            if (idx == -1) break;
            nodeIndex = idx;
            frag.clear();
        }
    }
    frag.clear();
    nodeIndex = 0;
    for(int i=0;i<l-1;i++) {
        if (i >= 1000) break;
        frag += x[l-i-1];
        int idx = revName.nodes[nodeIndex].connectionIndex(frag);
        if (idx != -1 && revName.finishable(idx)) {
            debug("Valid suffix length: %d", i+1);
            rainbowSuffix[i+1] = true;
        }
        if (i % 10 == 9) {
            if (idx == -1) break;
            nodeIndex = idx;
            frag.clear();
        }
    }
}

void query() {
    string handle;
    cin >> handle;
    int l = handle.length();
    getRainbowTable(handle);
    const int cs = l - 1000 > 1 ? l - 1000 : 1;
    const int ce = l - 1 < 1000 ? l - 1 : 1000;
    for(int i=cs;i<=ce;i++) {
        int prefixLength = i;
        int suffixLength = l - i;
        if (rainbowPrefix[prefixLength] && rainbowSuffix[suffixLength]) {
            cout << "Yes" << endl;
            return;
        }
    }
    
    cout << "No" << endl;
    return;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    input();
    while(q--) {
        query();
    }
    return 0;
}
