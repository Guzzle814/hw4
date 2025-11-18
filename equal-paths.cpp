#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int findDepth(Node* root)
{
    if (root == nullptr)
    {
      return 0;
    }
    if (root->left == nullptr && root->right == nullptr){
      return 1;
    }
    
    int leftDepth  = findDepth(root->left);
    int rightDepth = findDepth(root->right);

    if (leftDepth > rightDepth) return 1+leftDepth;
    else return 1+rightDepth;
}

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == nullptr)
    {
      return true;
    }
    else if(root->left == nullptr && root->right == nullptr)
    {
      return true;
    }
    else if(root->left!= nullptr && root->right == nullptr)
    {
      return equalPaths(root->left);
    }
    else if(root->left == nullptr && root->right != nullptr)
    {
      return equalPaths(root->right);
    }

    int leftDepth  = findDepth(root->left);
    int rightDepth = findDepth(root->right);
    if (leftDepth != rightDepth) 
    {
      return false;
    }
    return equalPaths(root->left) && equalPaths(root->right);  
}

