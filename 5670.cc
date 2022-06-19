#include <cstdio>
#include <iostream>
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

typedef pair<int, int> pii;
typedef long long llint;

int n;
struct TrieNode {
    string letter;
    bool isLastChar;
    int parentIndex;
    int depth;
    int childCount;
    int portal[26];
    TrieNode(string _letter, bool _isLastChar, int _parentIndex, int _depth)
        : letter(_letter),
        isLastChar(_isLastChar),
        parentIndex(_parentIndex),
        depth(_depth),
        childCount(0)
    {
        fill(begin(portal), end(portal), -1);
    }
};
vector<TrieNode> trie;
string a[100010];
int lastCharIndex[100010];

int input() {
    cin >> n;
    if (cin.eof()) return -1;
    for(int i=0;i<n;i++) {
        cin >> a[i];
    }
    return 0;
}
int insertTrie(const string &s) { // returns the index of the last character, on the trie
    int l = s.length();
    int currentTrieIndex = 0;
    for(int i=0;i<l;i++) {
        int charIdx = s[i] - 'a';
        if (trie[currentTrieIndex].portal[charIdx] == -1) {
            trie[currentTrieIndex].childCount++;
            string frag(1, s[i]);
            TrieNode newNode(frag, false, currentTrieIndex, trie[currentTrieIndex].depth + 1);
            trie.push_back(newNode);
            trie[currentTrieIndex].portal[charIdx] = trie.size() - 1;
            currentTrieIndex = trie[currentTrieIndex].portal[charIdx];
        } else {
            currentTrieIndex = trie[currentTrieIndex].portal[charIdx];
        }
    }
    TrieNode &finalNode = trie[currentTrieIndex];
    finalNode.isLastChar = true;
    return currentTrieIndex;
}
int getAutoCompleteDepth(int lastTrieIndex) {
    int currentIndex = lastTrieIndex;
    int depth = trie[currentIndex].depth;
    int effectiveDepth = 0;
    while(currentIndex != 0) {
        TrieNode &currentNode = trie[currentIndex];
        TrieNode &parentNode = trie[currentNode.parentIndex];
        // if currentNode is mergable to parentNode
        if (parentNode.childCount == 1 && !parentNode.isLastChar && parentNode.parentIndex != -1) {
            // get grandparent
            TrieNode &grandParentNode = trie[parentNode.parentIndex];
            // currentNode consumes the parentNode
            currentNode.letter = parentNode.letter + currentNode.letter;
            currentNode.parentIndex = parentNode.parentIndex;
            // unlink grandparent with parent and replace it with currentNode
            int grandParentPortalIndex = currentNode.letter[0] - 'a';
            grandParentNode.portal[grandParentPortalIndex] = currentIndex;
        } else {
            currentIndex = currentNode.parentIndex;
            effectiveDepth++;
        }
    }
    return effectiveDepth;
}

void dumpTrie(string frag, int currentIndex) {
#ifdef DEBUG
    if (frag.length() > 0) {
        debug("%s", frag.c_str());
    }
    TrieNode &currentNode = trie[currentIndex];
    for(int i=0;i<26;i++) {
        if (currentNode.portal[i] != -1) {
            TrieNode &childNode = trie[currentNode.portal[i]];
            dumpTrie(frag + childNode.letter, currentNode.portal[i]);
        }
    }
#endif
}

void solve() {
    for (int i=0;i<n;i++) {
        lastCharIndex[i] = insertTrie(a[i]);
    }
    debug("before path compression");
    dumpTrie("", 0);
    int sum = 0;
    for (int i=0;i<n;i++) {
        int depth = getAutoCompleteDepth(lastCharIndex[i]);
        sum += depth;
    }
    debug("after path compression");
    dumpTrie("", 0);
    debug("sum: %d", sum);
    cout << (double)sum/(double)n << "\n";
}
void init() {
    trie.clear();
    trie.push_back(TrieNode(string("$"), false, -1, 0));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cout << fixed;
    cout.precision(2);
    while(!input()) {
        init();
        solve();
    }
    return 0;
}
