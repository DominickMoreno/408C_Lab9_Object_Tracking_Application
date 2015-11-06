#ifndef HIST2D_H
#define HIST2D_H

#include"hist.h"
/*
 * Histogram 2D
 * Jacob Knapo
 * 408c
*/

/*Computes the histogram of the given 2d data set within a tile. 

int *head - pointer to 2d data set
int w - Width of tile
int h - hieght of tile
int stride - Number of pixels between two contigous rows
int *bins - pointer to ranges for each bin. bins must follow a specific 
pattern. r1 r2 r3 r4......   r1 and r2 are the min and max ranges for bin 1.
r3 and r4 are the min and max ranges for bin 2.
int m - number of bins
int *out- pointer to an array the size of m ints 

*/
void hist_2d(int *head,
			 int w,
			 int h,
			 int stride,
			 int *bins,
			 int m,
			 int *out);
#endif 
