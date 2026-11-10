#include <iostream>
#include <stack>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int d) {
        data = d;
        left = NULL;
        right = NULL;
    }
};

class BST {
    Node* root;
    int recCount;
    int iterCount;

    Node* insertNode(Node* r, int val) {
        if (r == NULL) return new Node(val);
        if (val < r->data) r->left = insertNode(r->left, val);
        else if (val > r->data) r->right = insertNode(r->right, val);
        return r;
    }

    void inorder(Node* r) {
        if (r == NULL) return;
        inorder(r->left);
        cout << r->data << " ";
        inorder(r->right);
    }

    //recursive range query, counts every comparison
    void rangeRec(Node* r, int low, int high) {
        if (r == NULL) return;
        recCount++;
        if (r->data > low) rangeRec(r->left, low, high);
        recCount++;
        if (r->data >= low) {
            recCount++;
            if (r->data <= high) cout << r->data << " ";
        }
        recCount++;
        if (r->data < high) rangeRec(r->right, low, high);
    }

    void destroy(Node* r) {
        if (r == NULL) return;
        destroy(r->left);
        destroy(r->right);
        delete r;
    }

public:
    BST() {
        root = NULL;
        recCount = 0;
        iterCount = 0;
    }

    ~BST() {
        destroy(root);
    }

    void insert(int val) {
        root = insertNode(root, val);
    }

    void display() {
        inorder(root);
        cout << endl;
    }

    void findPredSucc(int key) {
        Node* pred = NULL;
        Node* succ = NULL;
        Node* cur = root;

        //successor is the smallest key bigger than key
        while (cur != NULL) {
            if (key < cur->data) {
                succ = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }

        // predecessor is the biggest key smaller than key
        cur = root;
        while (cur != NULL) {
            if (key > cur->data) {
                pred = cur;
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }

        if (pred != NULL) cout << "Predecessor of " << key << ": " << pred->data << endl;
        else cout << "Predecessor of " << key << ": none" << endl;

        if (succ != NULL) cout << "Successor of " << key << ": " << succ->data << endl;
        else cout << "Successor of " << key << ": none" << endl;
    }

    void rangeRecursive(int low, int high) {
        recCount = 0;
        cout << "Range " << low << " to " << high << " (recursive): ";
        rangeRec(root, low, high);
        cout << endl;
        cout << "Comparisons (recursive): " << recCount << endl;
    }

    //iterative range query using a stack
    void rangeIterative(int low, int high) {
        iterCount = 0;
        stack<Node*> st;
        Node* cur = root;

        cout << "Range " << low << " to " << high << " (iterative): ";
        while (cur != NULL || !st.empty()) {
            while (cur != NULL) {
                iterCount++;
                if (cur->data >= low) {
                    st.push(cur);
                    cur = cur->left;
                } else {
                    cur = cur->right;
                }
            }
            if (st.empty()) break;
            cur = st.top();
            st.pop();
            iterCount++;
            if (cur->data > high) break;
            cout << cur->data << " ";
            cur = cur->right;
        }
        cout << endl;
        cout << "Comparisons (iterative): " << iterCount << endl;
    }
};
//main implementation
int main() {
    int arr[] = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
    int n = 11;

    BST tree;
    for (int i = 0; i < n; i++) {
        tree.insert(arr[i]);
    }

    cout << "Inorder: ";
    tree.display();

    tree.findPredSucc(40);

    tree.rangeRecursive(30, 70);
    tree.rangeIterative(30, 70);

    return 0;
}