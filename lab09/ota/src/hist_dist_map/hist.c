#include<stdlib.h>
#include<stdio.h>
#include"hist.h"

void hist(int *data, int n, int *bins, int m, int *out)
{
	int r1=0;
 	int r2=0;/*Temp ranges for each bin*/
	int range=0;
	int i=0;


	/*For each bin range calculate number of data points that fall within*/
	for(range=0; range<m;range++)
		{
			r1=bins[2*range];/*inner bound*/
			r2=bins[2*range+1];/*outer bound*/

			for(i=0;i<n;i++)
				{
					if(data[i]<bins[0] || data[i]>bins[2*m-1])
					{
						printf("ERROR - Data point %d is out of bin range\n",
						data[i]);
						exit(1);
					}
					if(data[i]>=r1 && data[i]<=r2)
					{
						//printf("Data: %d in range %d-%d\n", data[i],r1,r2);
						out[range]=out[range]+1;
				  	}
				}
		}
}


