#include <cstdio>
#include <cstring>

struct Node {
    Node *p;
    Node *l;
    Node *r;
    char key;
    int cnt;
    Node(char key): key(key), cnt(0), p(nullptr), l(nullptr), r(nullptr) {}
    bool isLeftChild() {
        if (!p) return false;
        return p->l == this;
    }
    bool isRightChild() {
        if (!p) return false;
        return p->r == this;
    }
};

struct SplayTree {
    Node *root = nullptr;
    SplayTree() {}
    void insert(char key) {
        //is mock.
        if (!root) {
            root = new Node(key);
            root->cnt = 1;
        } else {
            root->r = new Node(key);
            root->r->p = root;
            root->r->cnt = 1;
            splay(root->r);
        }
    }

    int safeCount(Node *p) {
        if (!p) return 0;
        return p->cnt;
    }

    char safeKey(Node *p) {
        if (!p) return '%';
        return p->key;
    }

    void rotate(Node *x) {
        if (!x->p) return;
        Node *p = x->p;
        Node *g = p->p;
        Node *c = nullptr;
        if (x->isLeftChild()) {
            if (g) {
                if (p->isLeftChild()) {
                    g->l = x;
                } else {
                    g->r = x;
                }
            }
            c = x->r;
            x->p = g;
            x->r = p;
            p->p = x;
            p->l = c;
            if (c) c->p = p;
            p->cnt = safeCount(p->l) + safeCount(p->r) + 1;
            x->cnt = safeCount(x->l) + safeCount(x->r) + 1;
        } else {
            if (g) {
                if (p->isLeftChild()) {
                    g->l = x;
                } else {
                    g->r = x;
                }
            }
            c = x->l;
            x->p = g;
            x->l = p;
            p->p = x;
            p->r = c;
            if (c) c->p = p;
            p->cnt = safeCount(p->l) + safeCount(p->r) + 1;
            x->cnt = safeCount(x->l) + safeCount(x->r) + 1;
        }
    }
    void splay(Node *x) {
        while(x->p) {
            Node *p = x->p;
            Node *g = p->p;
            if (g) {
                if ((p->isLeftChild() && x->isLeftChild()) || (p->isRightChild() && x->isRightChild())) {
                    rotate(p);
                } else {
                    rotate(x);
                }
            }
            rotate(x);
        }
        root = x;
    }
    Node* findKthElement(int k){
        Node *x = root;
        int originalK = k;
        while (true) {
            if (safeCount(x->l) <= k) {
                k -= safeCount(x->l);
                if (k == 0) {
                    break;
                } else {
                    k--;
                    x = x->r;
                }
            } else {
                x = x->l;
            }
        }
        splay(x);
        return x;
    }
    void preorder(Node *x) {
        if (x->l) {
            preorder(x->l);
        }
        printf("%c",x->key);
        if (x->r) {
            preorder(x->r);
        }
    }
    void swapThroughLeft(Node *p1, Node *p2) {
        while(p2->p != p1 && p2 != p1) {
            rotate(p2);
        }
        Node *target = p2->r;
        Node *minp = p2;
        while(minp->l) minp = minp->l;
        while(minp->p != p1) {
            rotate(minp);
        }
        minp->l = target;
        target->p = minp;
        p2->r = nullptr;
        minp->cnt = safeCount(minp->l) + safeCount(minp->r) + 1;
        p2->cnt = safeCount(p2->l) + safeCount(p2->r) + 1;
    }
    void swapThroughRight(Node *p1, Node *p2) {
        while(p2->p != p1 && p2 != p1) {
            rotate(p2);
        }
        Node *target = p2->l;
        Node *minp = p2;
        while(minp->r) minp = minp->r;
        while(minp->p != p1) {
            rotate(minp);
        }
        minp->r = target;
        target->p = minp;
        p2->l = nullptr;
        minp->cnt = safeCount(minp->l) + safeCount(minp->r) + 1;
        p2->cnt = safeCount(p2->l) + safeCount(p2->r) + 1;
    }
};

SplayTree *tree;
char x[100010];
int n;
int q;

int main() {
    tree = new SplayTree();
    scanf("%s",x);
    n = strlen(x);
    for(int i = 0; i < n; i++) {
        tree->insert(x[i]);
    }
    scanf("%d",&q);
    while(q--){
        int a,b,c;
        Node *p, *p2, *tmp;
        scanf("%d",&c);
        switch(c) {
        case 1:
            scanf("%d%d",&a,&b);
            if (a != 0) {
                p = tree->findKthElement(a-1);
                if (b == n-1) {
                    p2 = p;
                } else {
                    p2 = tree->findKthElement(b+1);
                }
            } else {
                continue;
            }
            tree->swapThroughLeft(p2, p);
            break;
        case 2:
            scanf("%d%d",&a,&b);
            if (a != 0) {
                if (b == n-1) {
                    continue;
                }
                p2 = tree->findKthElement(b+1);
                p = tree->findKthElement(a-1);
            } else {
                if (b == n-1) {
                    continue;
                }
                p = tree->findKthElement(b+1);
                p2 = p;
            }
            tree->swapThroughRight(p, p2);
            break;
        case 3:
            scanf("%d",&a);
            p = tree->findKthElement(a);
            printf("%c\n", p->key);
            break;
        }
    }
    return 0;
}