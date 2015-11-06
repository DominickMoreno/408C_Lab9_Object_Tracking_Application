#ifndef HIST_H
#define HIST_H
/*
 * Histogram
 * Jacob Knapo
 * 408c
*/

/*Computes the histogram of the given data set. Puts the histo gram into 
out. 
int *data - pointer to data set
int n - number of data points
int *bins - pointer to ranges for each bin. bins must follow a specific 
pattern. r1 r2 r3 r4......   r1 and r2 are the min and max ranges for bin 1.
r3 and r4 are the min and max ranges for bin 2.
int *out- pointer to an array the size of m ints 

*/
void hist(int *data, int n, int *bins, int m, int *out);
 
#endif
