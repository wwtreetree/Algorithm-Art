
#include "stats.h"



// initialize the private vectors so that, for each color,  entry
// (x,y) is the cumulative sum of the the color values from (0,0)
// to (x,y). Similarly, the sumSq vectors are the cumulative
// sum of squares from (0,0) to (x,y).
stats::stats(PNG & im){

/* Your code here!! */

    sumRed.clear();
    sumGreen.clear();
    sumBlue.clear();
    sumsqRed.clear();
    sumsqBlue.clear();
    sumsqGreen.clear();



    sumRed.resize(im.width(), (vector<long>) im.height());
    sumGreen.resize(im.width(), (vector<long>) im.height());
    sumBlue.resize(im.width(), (vector<long>) im.height());

    sumsqRed.resize(im.width(), (vector<long>) im.height());
    sumsqGreen.resize(im.width(), (vector<long>) im.height());
    sumsqBlue.resize(im.width(), (vector<long>) im.height());

    for(unsigned int i = 0; i < im.width(); i++) {

        long red=0;
        long green=0;
        long blue=0;
        long sqred=0;
        long sqgreen=0;
        long sqblue=0;

        for (unsigned int j = 0; j < im.height(); j++) {
            
            RGBAPixel * pixel = im.getPixel(i, j);

            red = red + pixel->r;
            green = green + pixel->g;
            blue = blue + pixel->b;

            sqred = sqred + pixel->r * pixel->r;
            sqgreen = sqgreen + pixel->g * pixel->g;
            sqblue = sqblue + pixel->b * pixel->b;
            
            // asignment
            sumRed[i][j] = red;
            sumGreen[i][j] = green;
            sumBlue[i][j] = blue;
            sumsqRed[i][j] = sqred;
            sumsqGreen[i][j] = sqgreen;
            sumsqBlue[i][j] = sqblue;

            if (i != 0) {
                sumRed[i][j] = sumRed[i][j] + sumRed[i-1][j];
                sumGreen[i][j]  = sumGreen[i][j] +  sumGreen[i-1][j];
                sumBlue[i][j] = sumBlue[i][j] + sumBlue[i-1][j];
                
                sumsqRed[i][j] += sumsqRed[i-1][j];
                sumsqGreen[i][j] += sumsqGreen[i-1][j];
                sumsqBlue[i][j] += sumsqBlue[i-1][j];
            }
        }

    }

}

/* returns the sums of all pixel values across all color channels.
	* useful in computing the variance of a square
	* PA3 function
	* @param channel is one of r, g, or b
	* @param ul is (x,y) of the upper left corner of the square
	* @param dim is log of side length of the square */
long stats::getSum(char channel, pair<int,int> ul, int dim){
    int width = pow(2, dim);
    int p1 = ul.first + width - 1;
    int p2 = ul.second + width - 1;
    pair<int,int> lr(p1, p2);
    if (channel == 'r') {
        return sumRed[p1][p2] - tbRemove(sumRed, ul, lr);
    }

    else if (channel =='g') {
        return sumGreen[p1][p2] - tbRemove(sumGreen, ul, lr);
    }

    else if (channel =='b') {
        return sumBlue[p1][p2] - tbRemove(sumBlue, ul, lr);
    }

    else {cout<<"XX_97__XX"<<endl;}

}



/* returns the sums of squares of all pixel values across all color channels.
	* useful in computing the variance of a square
	* PA3 function
	* @param channel is one of r, g, or b
	* @param ul is (x,y) of the upper left corner of the square
	* @param dim is the log of the side length of the square */
long stats::getSumSq(char channel, pair<int,int> ul, int dim){
/* Your code here!! */

    int width = pow(2, dim);

    int p1 = ul.first + width - 1;
    int p2 = ul.second + width - 1;

    pair<int,int> lr(p1, p2);
    if (channel == 'r') {
        return sumsqRed[lr.first][lr.second] - tbRemove(sumsqRed, ul, lr);
    }

    else if (channel =='g') {
        return sumsqGreen[lr.first][lr.second] - tbRemove(sumsqGreen, ul, lr);
    }

    else if (channel =='b') {
        return sumsqBlue[lr.first][lr.second] - tbRemove(sumsqBlue, ul, lr);
    }
}

// return the value tobe removed
long stats::tbRemove(vector< vector< long >> &sum, pair<int,int> ul, pair<int,int> lr){
    if (ul.first == 0 && ul.second == 0)

        return 0;

    else if (ul.second == 0)
        return sum[ul.first-1][lr.second];
    
    else if (ul.first == 0) 
        return sum[lr.first][ul.second-1];
    
    else
        return sum[lr.first][ul.second-1] 
        + sum[ul.first-1][lr.second] 
        - sum[ul.first-1][ul.second-1];
}

long stats::rectArea(int dim) {
    return pow(pow(2, dim),2);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int dim){
    //
//long sum = getSum('r',ul,dim) + getSum('g',ul,dim) +getSum('b',ul,dim);
//long sumSquare = getSumSq('r',ul,dim) + getSumSq('g',ul,dim) + getSumSq('b',ul,dim);
//cout << "sum " << sum << endl;
//cout << "sumSquare " << sumSquare << endl;
//cout << "rectArea " << rectArea(dim) << endl;
//
//return sumSquare - ((sum*sum) / rectArea(dim));
    double one = (double) getSumSq('r',ul,dim)-pow(getSum('r',ul,dim),2)/rectArea(dim);

    double two =  (double)getSumSq('g',ul,dim)-pow(getSum('g',ul,dim),2)/rectArea(dim);

    double three =(double)getSumSq('b',ul,dim)-pow(getSum('b',ul,dim),2)/rectArea(dim);
    //3174-2116
return one + two + three;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int dim){
    /* Your code here!! */
//rgb
    //cout << "getAVG" << endl;
    long redAvg = long(getSum('r',ul,dim)/rectArea(dim));
    long greenAvg = long(getSum('g',ul,dim)/rectArea(dim));
    long blueAvg = long(getSum('b',ul,dim)/rectArea(dim));

    return RGBAPixel(redAvg, greenAvg, blueAvg);
}
