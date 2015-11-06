#include<stdlib.h>
#include<stdio.h>
#include"hist2d.h"

void hist_2d(int *head, 
			 int w, 
			 int h, 
			 int stride, 
			 int *bins, 
			 int m, 
		     int *out)
{
	int i;
	int *tempOut = calloc(m , sizeof(int));	
	int k;

	if(tempOut == NULL)
		exit(1);

	/*Find the histogram for each row then add to the total histogram.*/
	for(i=0; i < h; i++)
	{
		hist(head + i*stride*2, w, bins, m, tempOut);	 
		/*Add histogram of current row to total histogram*/
		for(k=0;k<m;k++)
		{
			out[k] = out[k] + tempOut[k];
			tempOut[k] = 0;
		}
	}			
	free(tempOut);
}

