#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm> // For std::max

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node *left, *right, *parent;

    Node(int value) : data(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node *root;
    Node *TNULL;

    void initializeNULLNode(Node *node, Node *parent) {
        node->data = 0;
        node->color = BLACK;
        node->left = nullptr;
        node->right = nullptr;
        node->parent = parent;
    }

    void preOrderHelper(Node *node) {
        if (node != TNULL) {
            std::cout << node->data << " ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    void inOrderHelper(Node *node) {
        if (node != TNULL) {
            inOrderHelper(node->left);
            std::cout << node->data << " ";
            inOrderHelper(node->right);
        }
    }

    void postOrderHelper(Node *node) {
        if (node != TNULL) {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            std::cout << node->data << " ";
        }
    }

    void balanceInsert(Node *k) {
        Node *u;
        while (k->parent->color == RED) {
            if (k->parent == k->parent->parent->left) {
                u = k->parent->parent->right; // uncle
                if (u->color == RED) {
                    // case 3.1
                    k->parent->color = BLACK;
                    u->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        // case 3.2.2
                        k = k->parent;
                        leftRotate(k);
                    }
                    // case 3.2.1
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    rightRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->left; // uncle
                if (u->color == RED) {
                    // mirror case 3.1
                    k->parent->color = BLACK;
                    u->color = BLACK;
                    k->parent->parent->color = RED;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        // mirror case 3.2.2
                        k = k->parent;
                        rightRotate(k);
                    }
                    // mirror case 3.2.1
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    leftRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = BLACK;
    }

    void leftRotate(Node *x) {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node *x) {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void deleteFix(Node *x) {
        Node *s;
        while ( x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == RED) {
                    // case 3.1
                    s->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }
                if (s->left->color == BLACK && s->right->color == BLACK) {
                    // case 3.2
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->right->color == BLACK) {
                        // case 3.3
                        s->left->color = BLACK;
                        s->color = RED;
                        rightRotate(s);
                        s = x->parent->right;
                    }
                    // case 3.4
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                s = x->parent->left;
                if (s->color == RED) {
                    // mirror case 3.1
                    s->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }
                if (s->right->color == BLACK && s->left->color == BLACK) {
                    // mirror case 3.2
                    s->color = RED;
                    x = x->parent;
                } else {
                    if (s->left->color == BLACK) {
                        // mirror case 3.3
                        s->right->color = BLACK;
                        s->color = RED;
                        leftRotate(s);
                        s = x->parent->left;
                    }
                    // mirror case 3.4
                    s->color = x->parent->color;
                    x->parent->color = BLACK;
                    s->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    // Function to calculate the height of the tree
    int heightHelper(Node *node) {
        if (node == TNULL) {
            return -1; // Return -1 for null nodes
        }
        // Recursively calculate the height of left and right subtrees
        int leftHeight = heightHelper(node->left);
        int rightHeight = heightHelper(node->right);
        // Return the maximum height plus one
        return std::max(leftHeight, rightHeight) + 1;
    }

public:
    RedBlackTree() {
        TNULL = new Node(0);
        TNULL->color = BLACK;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    void preOrder() {
        preOrderHelper(this->root);
        std::cout << std::endl;
    }

    void inOrder() {
        inOrderHelper(this->root);
        std::cout << std::endl;
    }

    void postOrder() {
        postOrderHelper(this->root);
        std::cout << std::endl;
    }

    Node *getRoot() {
        return this->root;
    }

    void insert(int key) {
        Node *node = new Node(key);
        node->parent = nullptr;
        node->data = key;
        node->left = TNULL;
        node->right = TNULL;
        node->color = RED; // new node must be red

        Node *y = nullptr;
        Node *x = this->root;

        while (x != TNULL) {
            y = x;
            if (node->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->data < y->data) {
            y->left = node;
        } else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = BLACK;
            return;
        }

        if (node->parent->parent == nullptr) {
            return;
        }

        balanceInsert(node);
    }

    void deleteNode(int data) {
        Node *z = TNULL;
        Node *x, *y;
        while (root != TNULL) {
            z = root;
            while (z->left != TNULL && z->right != TNULL && z->data != data) {
                if (z->data <= data) {
                    z = z->right;
                } else {
                    z = z->left;
                }
            }

            if (z->data == data) {
                break;
            } else {
                std::cout << "Key not found in the tree" << std::endl;
                return;
            }
        }

        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == TNULL) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = treeMinimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == BLACK) {
            deleteFix(x);
        }
    }

    void rbTransplant(Node *u, Node *v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node *treeMinimum(Node *node) {
        while (node->left != TNULL) {
            node = node->left;
        }
        return node;
    }

    // Public function to get the height of the tree
    int height() {
        return heightHelper(this->root);
    }

    // Example function to print the height
    void printHeight() {
        std::cout << "Height of the Red-Black Tree: " << height() << std::endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <insert_file> <delete_file>" << std::endl;
        return 1;
    }

    RedBlackTree tree;

    std::ifstream insertFile(argv[1]);
    std::string line;
    while (std::getline(insertFile, line)) {
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            tree.insert(value);
        }
    }
    insertFile.close();

    std::cout << "In-order traversal after insertion: ";
    tree.inOrder();

    std::cout << "Height of the tree after insertion: ";
    tree.printHeight();

    std::ifstream deleteFile(argv[2]);
    while (std::getline(deleteFile, line)) {
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            tree.deleteNode(value);
        }
    }
    deleteFile.close();

    std::cout << "In-order traversal after deletion: ";
    tree.inOrder();

    std::cout << "Height of the tree after deletion: ";
    tree.printHeight();

    return 0;
}