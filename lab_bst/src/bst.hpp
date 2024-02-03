/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    Node* test = find(root, key);
    return test->value;
    // your code here
}

template <class K, class V>
struct BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
       
    if(subtree == NULL) {
        
        return subtree;
    }
    if (subtree->key == key) {
        return subtree;
    }
    else if(key < subtree->key) {
        return find(subtree->left, key);
    }
    else if( key > subtree->key) {
        return find(subtree->right, key);
    }
    return subtree;
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);

    // your code here
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
     if(!subtree) {
        subtree = new Node(key, value);
        return;
    }
    if(subtree->key == key) {
        return;
    }
    if(subtree->key >key) {
        if(subtree->left){
            insert(subtree->left,key,value);
        }
        else {
            subtree->left = new Node(key, value);
        }
    }
    else {
        if(subtree->right) {
            insert(subtree->right, key, value);
        }
        else {
            subtree->right = new Node(key, value);
        }
    }   
    // your code here
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    if(!first || !second) {
        return;
    }
    Node tmp = first;
    first->key = second->key;
    first->value = second->value;
    second->key = tmp.key;
    second->value = tmp.value;
    return;
    // your code here
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    remove(root,key);
}
template <class K, class V>
std::pair<K, V> BST<K,V>::predecessor(BST<K, V>::Node *& traverseRight){
    if(traverseRight->right){
        return predecessor (traverseRight->right);
    }
    V temp = traverseRight->value;
    K key = traverseRight->key;
    BST<K, V>::Node* left = traverseRight->left;
    delete(traverseRight);
    traverseRight = left;
    return std::pair<K,V>(key ,temp);
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
   if (!subtree) return;
   if (subtree->key > key) {
        remove (subtree->left, key);
    } else if (subtree->key < key) {
        remove (subtree->right, key);
    } else {
        if (!subtree->left && !subtree->right) {
            delete subtree;
            subtree = nullptr;
        } else if (subtree->left && !subtree->right){
            Node * tmp = subtree;
            subtree = subtree->left;
            delete tmp;
        } else if (subtree->right && !subtree->left) {
            Node * tmp = subtree;
            subtree = subtree->right;
            delete tmp;
        } else {
            Node * tmp = subtree;
            Node * above = nullptr;
            tmp = tmp->left;
            while(tmp->right) {
                above = tmp;
                tmp = tmp->right;
            }
            if (above) {
                above->right = nullptr;
            }
            if (tmp != subtree->left) 
            tmp->left = subtree->left;
            tmp->right = subtree->right;
            Node * tmp2 = subtree;
            if (subtree == root) root = tmp;
            subtree = tmp;
            delete tmp2;
            }
    }
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    BST<K, V> tree;
    for(auto pair : inList) {
        tree.insert(pair.first, pair.second);
    }
    return tree;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList){
    // your code here
    int n = inList.size();
    std::vector<int> histogram(n, 0);

    std::sort(inList.begin(), inList.end());

    do {
        BST<K, V> tree;
        for(auto pair : inList) {
            tree.insert(pair.first, pair.second);
        }
        int height = tree.height();
        histogram[height]++;

    }
    while(std::next_permutation(inList.begin(), inList.end()));
    return histogram;
}