/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}
/*
template <typename T>
void List<T>::_copy(const List<T>& other) {
    ListNode* current = other.head_;
    ListNode* last = nullptr;

    while (current != nullptr) {
        ListNode* newNode = new ListNode(current->data);

        if (last == nullptr) {
            head_ = newNode;
        } else {
            last->next = newNode;
            newNode->prev = last;
        }

        tail_ = newNode;

        current = current->next;
        last = newNode;
    }

    length_ = other.length_;
}
*/

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in mp_lists part 1
  return ListIterator(head_, tail_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  return ListIterator(NULL, tail_->next);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in mp_lists part 1
    ListNode* current = head_;
    while (current != nullptr) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }

    head_ = nullptr;
    tail_ = nullptr;
    length_ = 0;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = NULL;
  
  if (head_ != NULL) {
    head_ -> prev = newNode;
  }

  head_ = newNode; //added code

  if (tail_ == NULL) {
    tail_ = newNode;
  }
  

  length_+=1;

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T& ndata) {
    ListNode* newNode = new ListNode(ndata);

    newNode->prev = tail_;
    newNode->next = nullptr;

    if (tail_ != nullptr) {
        tail_->next = newNode;
    }

    tail_ = newNode;

    if (head_ == nullptr) {
        head_ = newNode;
    }

    length_+=1;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
   
    ListNode * curr = start;

    for (int i = 0; i < splitPoint; i++) {
        if (curr == NULL) {
            return NULL; 
        }
        curr = curr->next;
    }

    if (curr == NULL) {
        return NULL;
    }

    if (curr->prev != NULL) {
        curr->prev->next = NULL;
          
    }
    curr->prev = NULL;

    return curr; 
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */

  /// @todo Graded in part 1
template <typename T>
void List<T>::waterfall() {
    if (length_ < 3) {
        return;
    }

    ListNode* curr = head_->next;  
    ListNode* prev = head_;       

    while (curr != nullptr && curr->next != nullptr) {
        prev->next = curr->next;
        curr->next->prev = prev;

        curr->next = nullptr;
        tail_->next = curr;
        curr->prev = tail_;
        tail_ = curr;

        prev = prev->next;
        curr = prev->next;
    }
}



/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in mp lists part 2
  ListNode * lastnxt = endPoint->next;
  ListNode * startPrev = startPoint->prev;
  ListNode * temp; 
  ListNode * current = startPoint;
  

  while(current!= nullptr && current != endPoint ) {
    temp = current->next;
    current->next = current->prev;
    current->prev = temp;

    current = temp;
  }
 
  temp = startPoint->next;
  startPoint->next = lastnxt;
  startPoint->prev = temp;

  endPoint->next = endPoint->prev;
  endPoint->prev = startPrev;

  if(startPrev != nullptr) { 
    startPrev->next = endPoint; 
  }
  if(lastnxt != nullptr) { 
    lastnxt->prev = startPoint; 
  }
  
  if(head_ == startPoint) { 
    head_ = endPoint; 
  }
  if(tail_ == endPoint) { 
    tail_ = startPoint; 
  }
  
  temp = startPoint;
  startPoint = endPoint;
  endPoint = temp;
}


/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
    if (n <= 1 || head_ == nullptr || tail_ == nullptr) {
        return;
    }

    ListNode* blockStart = head_;
    ListNode* blockEnd = head_;
    ListNode* prevBlockEnd = nullptr;

    while (blockStart != nullptr) {
        for (int i = 1; i < n ; i++) {
            if(blockEnd != nullptr)
              blockEnd = blockEnd->next;
        }

        if (blockEnd == nullptr) {
            break;
        }

        ListNode* nextBlockStart = blockEnd->next;

        reverse(blockStart, blockEnd);

        if (prevBlockEnd != nullptr) {
            prevBlockEnd->next = blockStart;
            blockStart->prev = prevBlockEnd;
        } else {
            head_ = blockStart;
            head_->prev = nullptr;
        }

        //blockEnd->next = nextBlockStart;
        if (nextBlockStart != nullptr) {
            nextBlockStart->prev = blockEnd;
        } else {
            tail_ = blockEnd;
        }

        prevBlockEnd = blockEnd;
        blockStart = nextBlockStart;
        blockEnd = nextBlockStart;
    }

    if (blockStart != nullptr) {
        ListNode* actualBlockEnd = blockStart;
        while (actualBlockEnd->next != nullptr) {
            actualBlockEnd = actualBlockEnd->next;
        }

        reverse(blockStart, actualBlockEnd);

        if (prevBlockEnd != nullptr) {
            prevBlockEnd->next = blockStart;
            blockStart->prev = prevBlockEnd;
        } else {
            head_ = blockStart;
            head_->prev = nullptr;
        }

        tail_ = actualBlockEnd;
    }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}
template <typename T>
 typename List<T>::ListNode* insertSorted(typename List<T>::ListNode *head, T data) {
    typename List<T>::ListNode *newNode = new typename List<T>::ListNode();
    newNode->data = data;
    newNode->next = nullptr;

    if (head == nullptr || data < head->data) {
        newNode->next = head;
        return newNode;
    }

    typename List<T>::ListNode *current = head;
    while (current->next != nullptr && data > current->next->data) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    return head;
  }
/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
    ListNode* mergedHead = nullptr;
    ListNode* mergedTail = nullptr;

    // Initialize mergedHead with the smaller of the two heads
    if (first != nullptr && (second == nullptr || first->data < second->data)) {
        mergedHead = first;
        first = first->next;
    } else {
        mergedHead = second;
        second = second->next;
    }

    mergedTail = mergedHead;  // Initialize mergedTail

    // Merge the two lists
    while (first != nullptr && second != nullptr) {
        if (first->data < second->data) {
            mergedTail->next = first;
            first->prev = mergedTail;
            mergedTail = first;
            first = first->next;
        } else {
            mergedTail->next = second;
            second->prev = mergedTail;
            mergedTail = second;
            second = second->next;
        }
    }

    // Append any remaining elements from the first list, if any
    while (first != nullptr) {
        mergedTail->next = first;
        first->prev = mergedTail;
        mergedTail = first;
        first = first->next;
    }

    // Append any remaining elements from the second list, if any
    while (second != nullptr) {
        mergedTail->next = second;
        second->prev = mergedTail;
        mergedTail = second;
        second = second->next;
    }

    return mergedHead;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode* start, int chainLength) {
    // Base case: if the list has 0 or 1 elements, it's already sorted
    if (chainLength <= 1) {
        return start;
    }

    // Split the list into two halves
    int mid = chainLength / 2;
    ListNode* secondHalf = start;
    for (int i = 0; i < mid; i++) {
        secondHalf = secondHalf->next;
    }

    // Break the link between the two halves
    secondHalf->prev->next = nullptr;
    secondHalf->prev = nullptr;

    // Recursively sort the two halves
    start = mergesort(start, mid);
    secondHalf = mergesort(secondHalf, chainLength - mid);

    // Merge the sorted halves back together
    return merge(start, secondHalf);
}
