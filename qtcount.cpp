#include "qtcount.h"


/* function prunable returns true if all of the leaf nodes in root's
* subtree have color within tol of root->avg. Distance is measured as
* square distance over the color channels: (r1-r2)^2 + (g1-g2)^2 + (b1-b2)^2
* and "within" is inclusive of tol.
*/
bool qtcount::prunable(Node *root, int tol) {
    return prunableHelper(root, tol, root->avg);
}

// Helper
bool qtcount::prunableHelper(Node *root, int tol, RGBAPixel p) {
    /* Your code here */
    if (root == NULL) return false;

    if (root->NW != NULL) {
        return prunableHelper(root->NW, tol, p) && prunableHelper(root->NE, tol, p) && prunableHelper(root->SW, tol, p)
               && prunableHelper(root->SE, tol, p);
        //root->NW ==NULL && root->NE == NULL && root->SE ==NULL && root->SW == NULL
    } else {
        double dr = root->avg.r - p.r;
        double dg = root->avg.g - p.g;
        double db = root->avg.b - p.b;
        return pow(dr, 2) + pow(dg, 2) + pow(db, 2) <= tol;
    }
}

//bool qtcount::prunable(Node * root, Node* node, int tol) {
//    return prunable(root, root->NW, tol) && prunable(root, root->NW, tol) && ;



