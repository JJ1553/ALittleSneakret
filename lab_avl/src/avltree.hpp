/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    Node* temp = t->right;
    t->right = temp->left;
    temp->left = t;
    //t = temp;
    t->height = std::max(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;
    temp->height = std::max(heightOrNeg1(temp->left), heightOrNeg1(temp->right)) + 1;
    t = temp;
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    
    Node* temp = t->left;
    t->left = temp->right;
    temp->right = t;
    //t = temp;
    t->height = std::max(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;
    temp->height = std::max(heightOrNeg1(temp->left), heightOrNeg1(temp->right)) + 1;
    t = temp;
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
   
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree) {
    if (subtree == nullptr) {
        return;
    }

    // Step 1: Calculate the balance factor
    int balance = heightOrNeg1(subtree->right) - heightOrNeg1(subtree->left);

    // Step 2: Perform rotations if needed
    if (balance < -1) {
        if (heightOrNeg1(subtree->left->right) - heightOrNeg1(subtree->left->left) > 0) {
            // Left-Right Rotation
            rotateLeftRight(subtree);
        }
        else { rotateRight(subtree);}
        // Left Rotation
       
    } else if (balance > 1) {
        if (heightOrNeg1(subtree->right->right) - heightOrNeg1(subtree->right->left) < 0) {
            // Right-Left Rotation
            rotateRightLeft(subtree);
        }
        else {rotateLeft(subtree);}
        // Right Rotation
        
    }

    // Step 3: Update node heights
    subtree->height = std::max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right)) + 1;
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
    
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    if (subtree == nullptr) {
        subtree = new Node(key, value);
        subtree->height = 0;
        return;
    } else {
        if (key < subtree->key) {
            insert(subtree->left, key, value);
        } 
        else if (key > subtree->key) {
            insert(subtree->right, key, value);
        }

        // Update height of the current node
        //subtree->height = std::max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right)) + 1;

        // Rebalance the current node
        rebalance(subtree);
    }
    
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        remove(subtree->right, key);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
             delete(subtree);
            subtree = NULL;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            // Two-child remove
            Node* successor = subtree->left;
            while (successor->right) {
                successor = successor->right;
            }
            K temp = subtree->key;
            V temp2 = subtree->value;
            subtree->key = successor->key;
            subtree->value = successor->value;
             successor->key = temp;
            successor->value = temp2;
            remove(subtree->left, key);
            //rebalance(subtree->left);  // Rebalance the right subtree
        } else {
            // One-child remove
            if(!subtree->left && subtree->right){
                Node* temp = subtree->right;
                delete(subtree);
                subtree = temp;
            }
             if(subtree->left && !subtree->right){
                Node* temp = subtree->left;
                delete(subtree);
                subtree = temp;
            }
        }
          // Rebalance the current node after removal
    }
    rebalance(subtree);
}

