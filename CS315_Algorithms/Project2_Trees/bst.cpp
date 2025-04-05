#include <iostream>
#include <fstream>
#include <string>
// Binary Search Tree
struct Node {
    int data;
    Node* left;
    Node* right;
    Node* parent;
};

struct tree {
    Node* root;
};

tree* createTree() {
    tree* newTree = new tree;
    newTree->root = nullptr;
    return newTree;
}

Node* createNode(int value) {
    Node* newNode = new Node;
    newNode->data = value;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = nullptr;
    return newNode;
}

void insert(tree* bst, int value) {
    Node* newNode = createNode(value);
    Node* y = nullptr;
    Node* x = bst->root;
    while (x != nullptr) {
        y = x;
        if (newNode->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    newNode->parent = y;
    if (y == nullptr) {
        bst->root = newNode;
    } else if (newNode->data < y->data) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }
}

Node* Search(Node* x, int value) {
    if ((x == nullptr) || (value == x->data)) {
        return x;
    }
    if (value < x->data) {
        return Search(x->left, value);
    } else {
        return Search(x->right, value);
    }
}

Node* Min(Node* x) {
    while (x->left != nullptr) {
        x = x->left;
    }
    return x;
}

void Transplant(tree* bst, Node* u, Node* v) {
    if (u->parent == nullptr) {
        bst->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

void Delete(tree* bst, int value) {
    Node* z = Search(bst->root, value);
    if (z == nullptr) {
        std::cout << "Value " << value << " not found in the tree." << std::endl;
        return;
    }

    if (z->left == nullptr) {
        Transplant(bst, z, z->right);
    } else if (z->right == nullptr) {
        Transplant(bst, z, z->left);
    } else {
        Node* y = Min(z->right);
        if (y->parent != z) {
            Transplant(bst, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        Transplant(bst, z, y);
        y->left = z->left;
        y->left->parent = y;
    }
    delete z; // Free the memory of the deleted node
}

void inOrderTraversal(Node* node) {
    if (node != nullptr) {
        inOrderTraversal(node->left);
        std::cout << node->data << " ";
        inOrderTraversal(node->right);
    }
}

// Function to find the height of the tree
int height(Node* node) {
    if (node == nullptr) {
        return -1; // Return -1 for empty tree or return 0 if you want height of a single node tree to be 1
    }
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    return std::max(leftHeight, rightHeight) + 1; // Height of the current node
}

void readFromFile(tree* bst, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return;
    }
    int value;
    while (file >> value) {
        insert(bst, value);
    }
    file.close();
}

void deleteFromFile(tree* bst, const std::string& filename) {
    std::ifstream file(filename);
    if (! file) {
        std::cerr << "Could not open the file: " << filename << std::endl;
        return;
    }
    int value;
    while (file >> value) {
        Delete(bst, value);
    }
    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <insert_file> <delete_file>" << std::endl;
        return 1;
    }

    tree* bst = createTree();

    std::string filename1 = argv[1]; 
    readFromFile(bst, filename1);

    std::cout << "In-order traversal of the BST after insertion:" << std::endl;
    inOrderTraversal(bst->root);
    std::cout << std::endl;

    std::cout << "Height of the BST: " << height(bst->root) << std::endl;

    std::string filename2 = argv[2]; 
    deleteFromFile(bst, filename2);
    std::cout << "In-order traversal of the BST after deletion:" << std::endl;
    inOrderTraversal(bst->root);
    std::cout << std::endl;

    std::cout << "Height of the BST after deletion: " << height(bst->root) << std::endl;

    return 0;
}