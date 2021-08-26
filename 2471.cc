#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <stack>
#include <list>
#include <set>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

typedef pair<int, int> pii;
typedef long long llint;

char original[220];
int K;

class MobileNode {
private:
    list<MobileNode*> chain;
    string content;
    set<string> possibleBinaries;
    bool revertible;
    bool onlyContent;
public:
    MobileNode(string t, bool _revertible, bool _onlyContent = false): revertible(_revertible), onlyContent(_onlyContent) {
        if (onlyContent) {
            content = t;
            return;
        }
        int n = t.length();
        string currentString;
        int globalParenthesisStack = 0;
        for(int i=0;i<n;i++) {
            if (t[i] == '(') {
                if (currentString.length() > 0) {
                    auto staticMBNode = new MobileNode(currentString, false, true);
                    chain.push_back(staticMBNode);
                    currentString = "";
                }
                //skip until next node
                auto mbNode = new MobileNode(t.substr(i+1), true);
                chain.push_back(mbNode);
                int parenthesisStack = 0;
                for(;i < n;i++) {
                    if (t[i] == '(') {
                        parenthesisStack++;
                    } else if (t[i] == ')') {
                        parenthesisStack--;
                        if (parenthesisStack == 0) {
                            break;
                        }
                    }
                }
            } else if (t[i] == ')') {
                if (currentString.length() > 0) {
                    auto staticMBNode = new MobileNode(currentString, false, true);
                    chain.push_back(staticMBNode);
                    currentString = "";
                }
                return;
            } else {
                currentString += t[i];
            }
        }
        if (currentString.length() > 0) {
            auto staticMBNode = new MobileNode(currentString, false, true);
            chain.push_back(staticMBNode);
        }
    }
    void generate(list<MobileNode*>::iterator it, string prefix) {
        if (possibleBinaries.size() >= K) return;
        if (it == chain.end()) {
            possibleBinaries.insert(prefix);
            return;
        }
        auto nextIt = it;
        nextIt++;
        for(auto cand: (*it)->possibleBinaries) {
            generate(nextIt, prefix + cand);
        }
    }
    void reverseGenerate(list<MobileNode*>::reverse_iterator rit, string prefix) {
        if (possibleBinaries.size() >= 2*K) return;
        if (rit == chain.rend()) {
            possibleBinaries.insert(prefix);
            return;
        }
        auto nextRit = rit;
        nextRit++;
        if ((*rit)->onlyContent) {
            string reversed = (*rit)->content;
            reverse(reversed.begin(), reversed.end());
            reverseGenerate(nextRit, prefix + reversed);
        } else {
            for(auto cand: (*rit)->possibleBinaries) {
                reverseGenerate(nextRit, prefix + cand);
            }
        }
    }
    void generateAllPossibleMobiles() {
        if (onlyContent) {
            possibleBinaries.insert(content);
        } else {
            for(auto node: chain) {
                node->generateAllPossibleMobiles();
            }
            generate(chain.begin(), string());
            reverseGenerate(chain.rbegin(), string());
        }
    }
    void printKthString() {
        if (possibleBinaries.size() < K) {
            printf("NO\n");
        } else {
            auto it = possibleBinaries.begin();
            for(int i=1; i<K ;i++, it++);
            printf("%s\n", (*it).c_str());
        }
    }
    string dump() {
        if (onlyContent) {
            return content;
        }
        string res;
        for(auto mb: chain) {
            res += mb->dump();
        }
        return res;
    }
};
class MobileBinary {
private:
    string original;
    int n;
    MobileNode *root;
public:
    MobileBinary(char *s) {
        original = s;
        root = new MobileNode(original, false);
    }
    void printKthString() {
        root->generateAllPossibleMobiles();
        root->printKthString();
    }
    void dump() {
        debug("%s", root->dump().c_str());
    }
};

int main() {
    scanf("%s",original);
    scanf("%d",&K);
    auto mb = MobileBinary(original);
    mb.printKthString();
    return 0;
}
