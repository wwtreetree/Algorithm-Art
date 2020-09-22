#include "qtvar.h"

/* function prunable returns true if root->var is strictly less than
       * tol. That is, a subtree can be pruned if its variance isn't too
       * high.
       */
bool qtvar::prunable(Node * root, int tol) {
    /* Your code here! */
    return root->var < tol; //?

}
