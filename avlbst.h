#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key,Value>* target);
    void rotateRight(AVLNode<Key,Value>* target);
    AVLNode<Key,Value>* balanceChecker(AVLNode<Key,Value>* node);
    int balanceUpdates(AVLNode<Key,Value>* node);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    //empty AVL case
    if(this->root_ == nullptr)
    {
        AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        this->root_ = newNode;
        return;
    }
    //existing node case
    Node<Key,Value>* temp = this->internalFind(new_item.first);
    if(temp!=nullptr)
    {
        temp->setValue(new_item.second);
        return;
    }   

    //find the location
    AVLNode<Key, Value>* curr   = static_cast<AVLNode<Key,Value>*>(this->root_);
    AVLNode<Key, Value>* parent = static_cast<AVLNode<Key,Value>*>(this->root_);

    while(curr != nullptr)//reused the bst pattern
    {
        parent = curr;
        if(new_item.first < curr->getKey())
        {
            curr = curr->getLeft();
        }
        else
        {
            curr = curr->getRight();
        }
    }

    //attach the newNode 
    AVLNode<Key, Value>* newNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if(new_item.first < parent->getKey())
    {
        parent->setLeft(newNode);
    }
    else
    {
        parent->setRight(newNode);   
    }

    balanceUpdates(static_cast<AVLNode<Key,Value>*>(this->root_));

    while(true)
    {
      AVLNode<Key,Value>* target = balanceChecker(static_cast<AVLNode<Key,Value>*>(this->root_));
      if(target == nullptr)
      {
        break;
      }

      int bal = target->getBalance();
      int childBal;

      if(bal == 2)
      {
        AVLNode<Key,Value>* rHeavy = target->getLeft();
        
        
        if (rHeavy == nullptr)
        {
          childBal = 0;
        }
        else
        {
          childBal = rHeavy->getBalance();
        }

        if(childBal >= 0)
        {
          rotateRight(target);
        }
        else
        {
          rotateLeft(rHeavy);
          rotateRight(target);
        }
      }
      else if(bal == -2)
      {
        AVLNode<Key,Value>* lHeavy = target->getRight();
        
        if (lHeavy == nullptr)
        {
          childBal = 0;
        }
        else
        {
          childBal = lHeavy->getBalance();
        }

        if(childBal <= 0)
        {
          rotateLeft(target);
        }
        else
        {
          rotateRight(lHeavy);
          rotateLeft(target);
        }
      }
      balanceUpdates(static_cast<AVLNode<Key,Value>*>(this->root_));
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* target)
{
  AVLNode<Key,Value>* newSubRoot = target->getRight();
  AVLNode<Key,Value>* x = newSubRoot->getLeft();
  AVLNode<Key,Value>* parent = target->getParent();

  //update new subroot
  if(parent == nullptr)
  {
    this->root_ = newSubRoot;
    newSubRoot->setParent(nullptr);
  }
  else
  {
    if(parent->getLeft() == target)
    {
      parent->setLeft(newSubRoot);
    }
    else
    {
      parent->setRight(newSubRoot);
    }
    newSubRoot->setParent(parent);
  }
  //attach target to newSubRoot
  newSubRoot->setLeft(target);
  target->setParent(newSubRoot);
  //move x to be right child of target to balance
  target->setRight(x);
  if(x != nullptr)
  {
    x->setParent(target);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* target)
{
  AVLNode<Key,Value>* newSubRoot = target->getLeft();
  AVLNode<Key,Value>* x = newSubRoot->getRight();
  AVLNode<Key,Value>* parent = target->getParent();

  //update new subroot
  if(parent == nullptr)
  {
    this->root_ = newSubRoot;
    newSubRoot->setParent(nullptr);
  }
  else
  {
    if(parent->getLeft() == target)
    {
      parent->setLeft(newSubRoot);
    }
    else
    {
      parent->setRight(newSubRoot);
    }
    newSubRoot->setParent(parent);
  }
  //attach target to newSubRoot
  newSubRoot->setRight(target);
  target->setParent(newSubRoot);
  //move x to be Left child of target to balance
  target->setLeft(x);
  if(x != nullptr)
  {
    x->setParent(target);
  }
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    BinarySearchTree<Key,Value>::remove(key);

    if(this->root_ == nullptr) 
    {
      return;
    }

    balanceUpdates(static_cast<AVLNode<Key,Value>*>(this->root_));

    while(true)
    {
      AVLNode<Key,Value>* target = balanceChecker(static_cast<AVLNode<Key,Value>*>(this->root_));
      if(target == nullptr)
      {
        break;
      }

      int bal = target->getBalance();
      int childBal;

      if(bal == 2)
      {
        AVLNode<Key,Value>* rHeavy = target->getLeft();
        
        
        if (rHeavy == nullptr)
        {
          childBal = 0;
        }
        else
        {
          childBal = rHeavy->getBalance();
        }

        if(childBal >= 0)
        {
          rotateRight(target);
        }
        else
        {
          rotateLeft(rHeavy);
          rotateRight(target);
        }
      }
      else if(bal == -2)
      {
        AVLNode<Key,Value>* lHeavy = target->getRight();
        
        if (lHeavy == nullptr)
        {
          childBal = 0;
        }
        else
        {
          childBal = lHeavy->getBalance();
        }

        if(childBal <= 0)
        {
          rotateLeft(target);
        }
        else
        {
          rotateRight(lHeavy);
          rotateLeft(target);
        }
      }
      balanceUpdates(static_cast<AVLNode<Key,Value>*>(this->root_));
    }
    
}

template<class Key, class Value>
int AVLTree<Key, Value>::balanceUpdates(AVLNode<Key,Value>* node)
{
  if(node == nullptr) return 0;

  int left = balanceUpdates(node->getLeft());
  int right = balanceUpdates(node->getRight());

  node->setBalance(left-right);
  return 1 + std::max(left,right);
}


template<class Key, class Value>
AVLNode<Key,Value>* AVLTree<Key, Value>::balanceChecker(AVLNode<Key,Value>* node)
{
  if(node == nullptr) return nullptr;
  AVLNode<Key, Value>* target = balanceChecker(node->getLeft());
  if(target != nullptr)
  {
    return target;
  }
  target = balanceChecker(node->getRight());
  if(target != nullptr)
  {
    return target;
  }

  if(std::abs(node->getBalance())>=2) 
  {
    return node;
  }

  return nullptr;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
