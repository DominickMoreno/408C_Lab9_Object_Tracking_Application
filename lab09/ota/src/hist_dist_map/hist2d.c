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
	int k,j,l;


	if(tempOut == NULL)
		exit(1);

	printf("Tile:\n");
	/*Find the histogram for each row then add to the total histogram.*/
	for(i=0; i < h; i++)
	{
		for(l=0; l < w; l++)
		{
		
			printf("%jx  ", (head + i*w + i*stride + l));
		}
		
		hist(head + i * (w + stride) , w, bins, m, tempOut);	 
		for(k=0;k<m;k++)
		{
			out[k] = out[k] + tempOut[k];
			tempOut[k] = 0;
		}
		printf("\n");
	}			
	free(tempOut);
}

