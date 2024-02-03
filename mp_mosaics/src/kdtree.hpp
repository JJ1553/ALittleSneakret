/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first, const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    if(first[curDim] < second[curDim]) {
      return true;
    }
    else if(first[curDim] > second[curDim]) {
      return false;
    }
    else 
    {
      return first < second;
    }
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target, const Point<Dim>& currentBest, const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
    double distOne = 0;
    double distTwo = 0;

    for(int i = 0 ; i < Dim ; i++) {
        distOne += ((currentBest[i] - target[i]) * (currentBest[i] - target[i])); 
        distTwo += ((potential[i] - target[i]) * (potential[i] - target[i])); 
    }

    if(distTwo == distOne) {
      return potential < currentBest;
    }

     return (distTwo < distOne);
}


template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints) {
    if (newPoints.empty()) {
        size = 0;
        root = nullptr;
    } 
    else {
        vector<Point<Dim>> temp = newPoints;
        root = buildTree(temp, 0, temp.size() - 1, 0);
        size = temp.size();
    }
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildTree(vector<Point<Dim>>& points, int left, int right, int currDim) {
    
    if (left > right) {
        return nullptr;
    }

    int splitDim = currDim % Dim;

    int med = (left + right) / 2;
    select(points.begin() + left, points.begin() + right + 1, points.begin() + med, [splitDim](const Point<Dim>& p1, const Point<Dim>& p2) {
        return smallerDimVal(p1, p2, splitDim);
    });

    KDTreeNode* newNode = new KDTreeNode(points[med]);

    newNode->left = buildTree(points, left, med - 1, splitDim + 1);
    newNode->right = buildTree(points, med + 1, right, splitDim + 1);

    return newNode;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
    root = copyTree(other.root);
    size = other.size;
}
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copyTree(const KDTreeNode* subroot) {
    if (subroot == nullptr) {
        return nullptr;
    }

    KDTreeNode* newNode = new KDTreeNode(subroot->point);
    newNode->left = copyTree(subroot->left);
    newNode->right = copyTree(subroot->right);

    return newNode;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  if (this != &rhs) {
        clearTree(root);
        root = copyTree(rhs.root);
        size = rhs.size;
    }
    return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
    delTree(root);
}
template <int Dim>
void KDTree<Dim>::delTree(KDTreeNode* root) {
    if (root == nullptr) {
        return;
    }

    delTree(root->left);
    delTree(root->right);
    delete root;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
  
    /**
     * @todo Implement this function!
     */
    
   if (root == nullptr) {
        
        return Point<Dim>();
    }

   KDTreeNode* nearestNode = Helper(root, query, root, 0);
    
    return nearestNode->point;
    
   //return query;
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::Helper(KDTreeNode* currNode, const Point<Dim>& target, KDTreeNode* currBest, int currDim) const {
    
    if(currNode == nullptr){ 
      return nullptr;
    }
    if((currNode->left == nullptr && currNode->right == nullptr)) {
      return currNode; 
    }

   
    int splitDim = (currDim+1) % Dim;

    if(currNode->right != nullptr) {
      if(smallerDimVal(currNode->point, target, currDim)) {
          currBest = Helper(currNode->right, target, currNode, splitDim);

      }
    }
    if(currNode->left != nullptr) {
      if (smallerDimVal(target, currNode->point, currDim)) {
          currBest = Helper(currNode->left, target, currNode, splitDim);
      } 
    }
    

     if (shouldReplace(target, currBest->point, currNode->point)) {
        currBest = currNode;
    }

  int splitDim2 = (currDim+1) % Dim;
  double bound = 0;
  for(int i = 0; i < Dim; i++) {
    bound += (target[i] - currBest->point[i]) * (target[i] - currBest->point[i]);
  }
  double distance = (currNode->point[currDim] - target[currDim]) * (currNode->point[currDim] - target[currDim]);

  if (distance <= bound) {
        KDTreeNode* oppositeNode = currBest;
        if(currNode->right != nullptr) {
          if (smallerDimVal(target, currNode->point, currDim)) {
              oppositeNode = Helper(currNode->right, target, currBest, splitDim2);
          } 
        } 
        if(currNode->left != nullptr) {
          if(smallerDimVal(currNode->point, target, currDim))
            oppositeNode = Helper(currNode->left, target, currBest, splitDim2);
        }

        if (shouldReplace(target, currBest->point, oppositeNode->point)) {
            currBest = oppositeNode;
        }
    }

    return currBest;
    
}


template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */    
    /*
    if(start == end) {
      return;
    }
  */

 
    RandIter medInd = end-1; //index
    RandIter store = start;
     

    for(RandIter it = start; it < end-1 ; it++) {
        if(cmp(*it, *medInd)) {
          std::swap(*it, *store);
          

          store++;
        }
    }
    std::swap(*medInd, *store);
          
    medInd = store;

    if(k == medInd) {
      return;
    }
    else if(k < medInd) {
      select(start, medInd, k, cmp);
    }
    else {
      select(medInd + 1, end, k, cmp);
    }

}

