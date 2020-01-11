#include <cstdio>
#include <cstring>
#include <memory.h>
#include <vector>
using namespace std;

int n, T;
char B[6][6];
int check[6][6];
int dir[8][2];
int score[8] = { 0, 0, 1, 1, 2, 3, 5, 11 };

class trieNode {
public:
    char repChar;
    bool isWord = false;
    int parent = -1;
    int myIndex = -1;
    int lastFoundRound = -1;
    int childIndex[26];
    int length = 0;
    trieNode(char x, int l, int p, int m) {
        repChar = x;
        length = l;
        parent = p;
        myIndex = m;
        memset(childIndex, -1, sizeof(childIndex));
    }
};

vector <trieNode> trie;

int getChildIndex(trieNode &node, char x) {
    if (x > 'Z' || x < 'A') return -1;
    return node.childIndex[x - 'A'];
}

void insert(char word[]) {
    int l = strlen(word);
    int trieIndex = 0;
    for(int i=0;i<l;i++) {
        int idx = getChildIndex(trie[trieIndex], word[i]);
        if (idx == -1) {
            idx = trie.size();
            trieNode newNode(word[i], i+1, trieIndex, idx);
            trie.push_back(newNode);
            trie[trieIndex].childIndex[word[i] - 'A'] = idx;
        }
        trieIndex = idx;
        if (i == l - 1) {
            trie[trieIndex].isWord = true;
        }
    }
}

int maxLength;
int totalScore;
int totalWords;
char maxWord[10];

void traverse(int x, int y, int l, int round, trieNode &node, char word[]) {
    check[x][y] = 1;
    if (node.isWord && node.lastFoundRound != round) {
        if (maxLength < node.length) {
            maxLength = node.length;
            strcpy(maxWord, word);
        } else if (maxLength == node.length) {
            int win = 0;
            for(int i=0;i<maxLength;i++) {
                if (maxWord[i] > word[i]) {
                    win = 1;
                    break;
                } else if(maxWord[i] < word[i]) {
                    win = -1;
                    break;
                }
            }
            if (win == 1) {
                strcpy(maxWord, word);
            }
        }
        totalScore += score[node.length - 1];
        totalWords++;
        node.lastFoundRound = round;
    }
    int nx, ny;
    for(int i=0;i<8;i++) {
        int nx = x + dir[i][0];
        int ny = y + dir[i][1];
        if (check[nx][ny]) continue;
        int idx = getChildIndex(node, B[nx][ny]);
        if (idx != -1){
            word[l] = B[nx][ny];
            traverse(nx, ny, l + 1, round, trie[idx], word);
            word[l] = 0;
        }
    }
    check[x][y] = 0;
}

int main() {
    trieNode root('%', 0, -1, 0);
    trie.push_back(root);
    int i, j, k = 0;
    for(i=-1;i<=1;i++) {
        for(j=-1;j<=1;j++) {
            if (!i && !j) continue;
            dir[k][0] = i;
            dir[k][1] = j;
            k++;
        }
    }
    scanf("%d",&n);
    char wd[10];
    for(i=0;i<n;i++) {
        scanf("%s", wd);
        insert(wd);
    }
    scanf("%d",&T);
    for(int Ti = 0; Ti < T; Ti++) {
        for(i=1;i<=4;i++) {
            scanf("%s",B[i]+1);
        }
        char word[10];
        memset(word, 0, sizeof(word));
        for(i=1;i<=4;i++) {
            for(j=1;j<=4;j++) {
                int childIndex = getChildIndex(trie[0], B[i][j]);
                if (childIndex != -1) {
                    word[0] = B[i][j];
                    traverse(i, j, 1, Ti, trie[childIndex], word);
                    word[0] = 0;
                }
            }
        }
        printf("%d %s %d\n", totalScore, maxWord, totalWords);
        maxLength = 0;
        totalScore = 0;
        totalWords = 0;
    }
}