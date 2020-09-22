
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"

using namespace std;


// Node constructor, given.
quadtree::Node::Node(pair<int,int> ul, int d,RGBAPixel a, double v)
	:upLeft(ul),dim(d),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

// quadtree destructor, given.
quadtree::~quadtree() {
	clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree & other) {
	copy(other);
}
// quadtree assignment operator, given.
quadtree & quadtree::operator=(const quadtree & rhs) {
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

/**
    * Constructor that builds a quadtree out of the given PNG.
    * Extract the largest 2^k x 2^k square from the upper left
    * corner of the image, and use it to build a 4-ary tree,
    * described below.
    *
    * Every leaf in the tree corresponds to a pixel in the PNG.
    * Every non-leaf node corresponds to a 2^k x 2^k square of pixels
    * in the original PNG, represented by an (x,y) pair for the
    * upper left corner of the square and an integer dimension, k.
    * In addition, the Node stores a pixel representing the average
    * color over the square. It also stores a measure of color
    * variability over the square. The formula for this variability
    * is found in the writeup for the assignment.
    *
    * Every node's children correspond to a partition
    * of the node's square into four smaller squares. The
    * current node's square is split in half horizontally
    * and vertically to produce the 4 children.
    *
    * The quadtree object also stores the edge length of the largest
    * square in member variable "edge".
    */

quadtree::quadtree(PNG & imIn) {
    /* Your code here! */
    int sideNumber = min(log2(imIn.height()), log2(imIn.width()));
    int sideLength = pow(2, sideNumber);

    stats s = stats(imIn);
    pair<int, int> ul = {0, 0};
    int dim = log2(sideLength);
    edge = sideLength;
    root = buildTree(s, ul, dim);
}

/**
    * Private helper function for the constructor. Recursively builds
    * the tree according to the specification of the constructor.
    * @param s Contains the data used to calc avg and var
    * @param ul upper left point of current node's square.
    * @param dim reflects the size of the current square
      */

quadtree::Node * quadtree::buildTree(stats & s, pair<int,int> ul, int dim) {
    /* Your code here! */

    Node *n = new Node(ul, dim, s.getAvg(ul, dim), s.getVar(ul, dim));

    if (dim != 0) {
        pair<int, int> NW = ul;
        pair<int, int> NE = {ul.first + pow(2, dim - 1), ul.second};
        pair<int, int> SE = {ul.first + pow(2, dim - 1), ul.second + pow(2, dim - 1)};
        pair<int, int> SW = {ul.first, ul.second + pow(2, dim - 1)};

        n->NW = buildTree(s, NW, dim - 1);
        n->NE = buildTree(s, NE, dim - 1);
        n->SE = buildTree(s, SE, dim - 1);
        n->SW = buildTree(s, SW, dim - 1);
	}

	return n;
}

/**
    * Render returns a PNG image consisting of the pixels
    * stored in the tree. It may be used on pruned trees. Draws
    * every leaf node's square onto a PNG canvas using the
    * average color stored in the node.
    */

PNG quadtree::render() {
    /* Your code here! */
    PNG im(edge, edge);
    renderHelper(root, im);
    return im;
}

//小的正方形一个颜色，然后包含他大点的正方形另一个颜色，但是颜色里面也会有小正方形的影响？？
void quadtree::renderHelper(Node* curr, PNG& im){
	if(curr->NW == NULL){
        
        // //the same to write curr->NE->upLeft.first???
        int w = pow(2, curr->dim);
        // *im.getPixel(curr->upLeft.first, curr->upLeft.second) = curr->avg; //NW
        //*im.getPixel(curr->upLeft.first + 1, curr->upLeft.second) = curr->avg; //NE
        //*im.getPixel(curr->upLeft.first, curr->upLeft.second + 1) = curr->avg; //SW
        //*im.getPixel(curr->upLeft.first + 1, curr->upLeft.second + 1) = curr->avg; //SE
		

        for(int i = curr->upLeft.first; i < curr->upLeft.first+w; i++){
            //cout<<"l1"<<endl;
            //cout<<"i= "<<i<<endl;
            for(int j = curr->upLeft.second; j < curr->upLeft.second+w; j++){
                //cout<<"l2"<<endl;
                //cout<<"j= "<<i<<endl;
                RGBAPixel *pixel = im.getPixel(i, j);
                *pixel = curr->avg;
			}
		}
    } else {
        renderHelper(curr->NW, im);
        renderHelper(curr->NE, im);
		renderHelper(curr->SW, im);
        renderHelper(curr->SE, im);
    }
}

/*
    * The idealPrune function can be considered to be the inverse
    * of the pruneSize function. It takes as input a number of leaves
    * and returns the minimum tolerance that would produce no more than
    * that many leaves upon a prune. It does not change the structure
    * of the tree.
      */
int quadtree::idealPrune(int leaves) {
    /* Your code here! */

    return idealPruneHelper(leaves, 0, 1000000);

}


int quadtree::idealPruneHelper(int leaves, int lower, int upper) {
    int mid = (lower + upper) / 2;
    int midSize = pruneSize(mid);

    if (lower > upper) return lower;
    else {
        if (midSize <= leaves) {
            return idealPruneHelper(leaves, lower, mid-1);
        }
        else {
            return idealPruneHelper(leaves, mid + 1, upper);
        }
    }

}

/*
    * The pruneSize function takes a tolerance as input, and returns
    * the number of leaves that would result _if_ the tree were to
    * be pruned with that tolerance. Consistent with the definition
    * of prune, a node is counted if prunable returns true.
    *
    * You may want a recursive helper function for this one.
    */
int quadtree::pruneSize(int tol){
	return pruneSizeHelper(tol, root);
}

//Helper
int quadtree::pruneSizeHelper(int tol, Node * & curr) {
	int sum = 0;
    if (curr == NULL) return 0;

	if (root->NW != NULL && !prunable(curr, tol)) {
			sum = sum + pruneSizeHelper(tol, curr->NW);
			sum = sum + pruneSizeHelper(tol, curr->NE);
			sum = sum + pruneSizeHelper(tol, curr->SW);
			sum = sum + pruneSizeHelper(tol, curr->SE);
    }
    else{ sum = 1; }

	return sum;
}


/*
   *  Prune function trims subtrees as high as possible in the tree.
   *  A subtree is pruned (cleared) if the prunable function returns true.
   *  Pruning criteria should be evaluated on the original tree, not
   *  on a pruned subtree. (we only expect that trees would be pruned once.)
   *
   * You may want a recursive helper function for this one.
   */
void quadtree::prune(int tol){
	pruneHelper(root, tol);
}

// Helper
void quadtree::pruneHelper(Node* curr, double tol){
	if (curr == NULL) return;

	if(curr->NW != NULL && prunable(curr, tol)){
		clearHelper(curr->NW);
		clearHelper(curr->NE);
		clearHelper(curr->SW);
		clearHelper(curr->SE);
        curr->NW = NULL;
        curr->NE = NULL;
        curr->SE = NULL;
        curr->SW = NULL;
        return;
	}

	if (curr->NW != NULL){
		pruneHelper(curr->NW,tol);
		pruneHelper(curr->NE,tol);
		pruneHelper(curr->SW,tol);
		pruneHelper(curr->SE,tol);
	}
}



/**
    * Destroys all dynamically allocated memory associated with the
    * current quadtree class. Complete for PA3.
    * You may want a recursive helper function for this one.
    */
void quadtree::clear() {
    /* your code here */
	clearHelper(root);
    root = NULL;
	edge = 0;
}


void quadtree::clearHelper(Node * & curr) {
	if (curr == NULL) return;

	if (curr->NW != NULL){
		clearHelper(curr->NW);
		curr->NW = NULL;
		clearHelper(curr->NE);
		curr->NE = NULL;
        clearHelper(curr->SW);
		curr->SW = NULL;
		clearHelper(curr->SE);
		curr->SE = NULL;
	}
	delete curr;
	curr = NULL;

}

/**
    * Complete for pa3
    * Copies the parameter other quadtree into the current quadtree.
    * Does not free any memory. Called by copy constructor and op=.
    * You may want a recursive helper function for this one.
    * @param other The quadtree to be copied.
    */

void quadtree::copy(const quadtree & orig){
    /* your code here */
	Node* n = new Node(orig.root->upLeft, orig.root->dim, orig.root->avg, orig.root->var);
	edge = orig.edge;
	if((orig.root->NW) != NULL){
		n->NW = copy(orig.root->NW);
		n->NE = copy(orig.root->NE);
		n->SW = copy(orig.root->SW);
		n->SE = copy(orig.root->SE);
	}
}

//为什么要加quadtree?
quadtree::Node* quadtree::copy(const Node * orig){
	Node* n = new Node(orig->upLeft, orig->dim, orig->avg, orig->var);
	if((orig->NW) != NULL){
		n->NW = copy(orig->NW);
		n->NE = copy(orig->NE);
		n->SW = copy(orig->SW);
		n->SE = copy(orig->SE);
	}
	return n;
}

