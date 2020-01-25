#include <cstdio>
#include <cassert>
#include <cstring>

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", \
__FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

char x[100010];
int n;
int q;

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
    void rebuildCount() {
        int newCount = 1;
        if (this->l) {
            newCount += this->l->cnt;
        }
        if (this->r) {
            newCount += this->r->cnt;
        }
        this->cnt = newCount;
    }
    Node* discardLeftChild() {
        if (!this->l) return this->l;
        Node *leftChild = this->l;
        this->l = nullptr;
        leftChild->p = nullptr;
        rebuildCount();
        return leftChild;
    }
    void ownLeftChild(Node *child) {
        assert(!this->l);
        if (!child) return;
        child->p = this;
        this->l = child;
        rebuildCount();
    }
    Node* discardRightChild() {
        if (!this->r) return this->r;
        Node *rightChild = this->r;
        this->r = nullptr;
        rightChild->p = nullptr;
        rebuildCount();
        return rightChild;
    }
    void ownRightChild(Node *child) {
        assert(!this->r);
        if (!child) return;
        child->p = this;
        this->r = child;
        rebuildCount();
    }
};

struct SplayTree {
    Node *root = nullptr;
    SplayTree() {}
    void insert(char key) {
        //is mock.
        if (!root) {
            debug("No root detected. %c is the initial key.", key);
            root = new Node(key);
            root->cnt = 1;
        } else {
            debug("%c is joining as a right child of the root node.", key);
            Node *x = new Node(key);
            root->ownRightChild(x);
            splay(x);
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

    void preorder(Node *x) {
        if (x->l) {
            preorder(x->l);
        }
        fprintf(stderr, "%c",x->key);
        if (x->r) {
            preorder(x->r);
        }
    }

    void rotateSubtree(Node *p, Node *x) {
        // assertion: say no if p is not x's parent.
        assert(x->p == p);

        bool lefty = x->isLeftChild();
        // step 2. parent discards x
        if (lefty) {
            p->discardLeftChild();
        } else {
            p->discardRightChild();
        }

        // step 3. x discards proper child
        Node *xChild = nullptr;
        if (lefty) {
            xChild = x->discardRightChild();
        } else {
            xChild = x->discardLeftChild();
        }
        // step 4. parent owns xChild as left child
        if (lefty) {
            p->ownLeftChild(xChild);
        } else {
            p->ownRightChild(xChild);
        }
        // step 5. x owns p as right child
        if (lefty) {
            x->ownRightChild(p);
        } else {
            x->ownLeftChild(p);
        }
    }

    void rotate(Node *x) {
        if (!x->p) {
            debug("x[key: %c] is a root node.", x->key);
            return;
        }
        Node *p = x->p;
        Node *gp = p->p;
        if (gp) {
            bool lefty = p->isLeftChild();
            // step 1. grandparent discards parent
            if (lefty) {
                p = gp->discardLeftChild();
            } else {
                p = gp->discardRightChild();
            }
            rotateSubtree(p, x);
            // step 6. gp owns x as proper child
            if (lefty) {
                gp->ownLeftChild(x);
            } else {
                gp->ownRightChild(x);
            }
        } else {
            rotateSubtree(p, x);
            root = x;
        }
    }

    void splay(Node *x) {
        while(x->p) {
            Node *p = x->p;
            Node *gp = p->p;
            if (gp) {
                if (p->isLeftChild() == x->isLeftChild()) {
                    rotate(p);
                } else {
                    rotate(x);
                }
            }
            rotate(x);
        }
    }

    Node* findKthElement(int k) {
        k++;
        Node *curr = this->root;
        while (k > 0) {
            if (curr->l) {
                if (curr->l->cnt < k) {
                    k -= curr->l->cnt;
                    if (k == 1) {
                        break;
                    }
                    k--;
                    assert(curr->r);
                    curr = curr->r;
                } else {
                    curr = curr->l;
                }
            } else {
                if (k == 1) {
                    break;
                }
                k--;
                assert(curr->r);
                curr = curr->r;
            }
        }
        splay(curr);
        return curr;
    }

    void moveToLeftmost(int a, int b) {
        if (b == n-1) {
            Node *p = findKthElement(a-1);
            Node *r = root->discardRightChild();
            findKthElement(0);
            root->ownLeftChild(r);
        } else {
            Node *p = findKthElement(a-1);
            Node *q = findKthElement(b+1);
            while(p->p != root) {
                rotate(p);
            }
            Node *r = p->discardRightChild();
            findKthElement(0);
            root->ownLeftChild(r);
        }
    }

    void moveToRightmost(int a, int b) {
        if (a == 0) {
            Node *p = findKthElement(b+1);
            Node *l = root->discardLeftChild();
            int maxNode = n - l->cnt - 1;
            findKthElement(maxNode);
            root->ownRightChild(l);
        } else {
            Node *p = findKthElement(b+1);
            Node *q = findKthElement(a-1);
            while(p->p != root) {
                rotate(p);
            }
            Node *l = p->discardLeftChild();
            int maxNode = n - l->cnt - 1;
            findKthElement(maxNode);
            root->ownRightChild(l);
        }
    }
};

SplayTree *tree;

int main() {
    tree = new SplayTree();
    scanf("%s",x);
    n = strlen(x);
    debug("insertion");
    for(int i = 0; i < n; i++) {
        tree->insert(x[i]);
    }
    debug("success");
#ifdef DEBUG
    tree->preorder(tree->root);
#endif
    debug("");
    scanf("%d",&q);
    while(q--){
        int a,b,c;
        Node *p, *p2, *tmp;
        scanf("%d",&c);
        switch(c) {
        case 1:
            scanf("%d%d",&a,&b);
            debug("%d %d %d", c, a, b);
            if (a != 0) {
                tree->moveToLeftmost(a, b);
            }
            break;
        case 2:
            scanf("%d%d",&a,&b);
            debug("%d %d %d", c, a, b);
            if (b != n-1) {
                tree->moveToRightmost(a, b);
            }
            break;
        case 3:
            scanf("%d",&a);
            debug("%d %d", c, a);
            Node *p = tree->findKthElement(a);
            printf("%c\n",p->key);
            break;
        }
        debug("preordering");
#ifdef DEBUG
        tree->preorder(tree->root);
#endif
        debug("");
    }
    return 0;
}