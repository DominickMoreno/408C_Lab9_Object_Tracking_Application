/*******************************************************************************
@ddblock_begin copyright
Copyright (c) 1997-2015
Maryland DSPCAD Research Group, The University of Maryland at College Park 
Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the above
copyright notice and the following two paragraphs appear in all copies
of this software.
IN NO EVENT SHALL THE UNIVERSITY OF MARYLAND BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
THE UNIVERSITY OF MARYLAND HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
THE UNIVERSITY OF MARYLAND SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
MARYLAND HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.
@ddblock_end copyright
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "lide_c_fifo.h"
#include "objtr_hist_dist_map.h"
#include "lide_c_util.h"

/*******************************************************************************
INNER PRODUCT STRUCTURE DEFINITION
*******************************************************************************/

struct _lide_c_objtr_hist_dist_map_context_struct { 
#include "lide_c_actor_context_type_common.h"

    /* Histogram_map variables. */
    int *histogram;
	int *bins;
	int bin_count;
	int W,H,Tw,Th;

	 

    /* Input ports. */
    lide_c_fifo_pointer ref;
    lide_c_fifo_pointer hist;

    /* Output port. */
    lide_c_fifo_pointer out;
};

/*******************************************************************************
IMPLEMENTATIONS OF INTERFACE FUNCTIONS.
*******************************************************************************/

/* This function code (and other code in this file) is formatted with extra 
line breaks so that it can be cut and pasted more conveniently into 
presentation slides. 
*/
lide_c_objtr_hist_dist_map_context_type *lide_c_objtr_hist_dist_map_new(
        lide_c_fifo_pointer ref, 
        lide_c_fifo_pointer hist, 
		int *histogram,
		int *bins,
		int bin_count,
		int W,
		int H,
		int Tw,
		int Th,
        lide_c_fifo_pointer out) { 


    lide_c_objtr_hist_dist_map_context_type *context = NULL;

    context = lide_c_util_malloc(
            sizeof(lide_c_objtr_hist_dist_map_context_type));
    context->mode = 
            lide_c_objtr_hist_dist_map_MODE_1;

    context->enable = (lide_c_actor_enable_function_type)
            lide_c_objtr_hist_dist_map_enable;

    context->invoke = (lide_c_actor_invoke_function_type)
            lide_c_objtr_hist_dist_map_invoke;

    context->ref = ref;
    context->hist = hist;
    context->histogram = NULL;
    context->bins = bins;
    context->bin_count = bin_count;
    context->W = W;
    context->H = H;
    context->Tw = Tw;
    context->Th = Th;
    context->out = out;

    return context;
}

boolean lide_c_objtr_hist_dist_map_enable(
        lide_c_objtr_hist_dist_map_context_type *context) {
	int out_size;
    boolean result = FALSE;

    switch (context->mode) {
        case lide_c_objtr_hist_dist_map_MODE_1:
            result = (lide_c_fifo_population(context->hist) ==
				context->bin_count);
            break;
        case lide_c_objtr_hist_dist_map_MODE_2:
			out_size = (context->W - context->Tw + 1) * (context->H -
				context->Th + 1);
            result = (out_size < lide_c_fifo_capacity(context->out) -
				lide_c_fifo_population(context->out)) &&
				(lide_c_fifo_population(context->ref) >= (context->W * context->H));
            break;
        default:
                result = FALSE;
                break;
    }
    return result;
}

static void compute_histogram(lide_c_objtr_hist_dist_map_context_type *context,
						  int *ref_image,
					      int *tile_hist)
{
	int i, j, k, upper_bound, lower_bound; 
	int width = context->Tw;
	for(i = 0; i < context->H; i++) {
	/*Define token  out size*/
		for(j = 0; j < context->W; j++) {
			for(k = 0; k < 2*context->bin_count; k += 2) {
			   
				lower_bound = context->bins[k];
				upper_bound = context->bins[k + 1];

				if(((k == 0) && (ref_image[i*width + j] < lower_bound)) ||
				   ((k == ((2*context->bin_count)-2)) && (ref_image[i*width + j] > upper_bound))) {
					printf("Value %d out of range.\n", ref_image[i + j]);
					break;
				}

				if((ref_image[i*width + j] >= lower_bound) &&
				   (ref_image[i*width + j] <= upper_bound)) {
					printf("%d is between %d and %d, adding to bin.\n", ref_image[i*width + j], lower_bound, upper_bound);
					tile_hist[(k/2)]++;
					break;
				}
			}
		}   
	}
}

static int compute_hist_dist(lide_c_objtr_hist_dist_map_context_type *context,
				 int *tile_hist)
{
	int i, dist = 0;

	for(i = 0; i < context->bin_count; i++)
	{
		dist += (context->histogram[i] + tile_hist[i]) * 
                (context->histogram[i] + tile_hist[i]);
	}
 	return dist;
}


void lide_c_objtr_hist_dist_map_invoke(lide_c_objtr_hist_dist_map_context_type *context) {
    int i, k, out_pos;
	int out_size;
    int *out;
	int *tile_hist;
    int **ref_image;

    printf("in invoke\n");
    switch (context->mode) {
        case lide_c_objtr_hist_dist_map_MODE_1:
			/*Take in histogram from hist_gen actor*/ 
			context->histogram = malloc(sizeof(int)*
				context->bin_count);		
            for (i = 0; i < context->bin_count; i++) {
                lide_c_fifo_read(context->hist, &(context->histogram[i]));
            }
					
            context->mode = lide_c_objtr_hist_dist_map_MODE_2;
            break;

        case lide_c_objtr_hist_dist_map_MODE_2:
			/*Define token  out size*/
			out_size = (context->W - context->Tw + 1) * (context->H -
				context->Th + 1);
            out = lide_c_util_malloc(sizeof(int) * out_size);
			
			/*Get reference image from source1*/
			ref_image = malloc(sizeof(int) * context->H);
			*ref_image = malloc(sizeof(int) * context->W);
			for(i = 0; i < context->H; i++)
			{
				for(k = 0; k < context->W; k++)
				{
					lide_c_fifo_read(context->ref, &(ref_image[i][k]));
				}
			}

			/*Find histogram for each tile of ref_image
			  then compute the hist_dist and store in out*/
			tile_hist = malloc(sizeof(int) * context->Tw * context->Th);	
			for(i = 0; i < (context->H - context->Th + 1); i++)
			{
				for(k  = 0; k < (context->W - context->Tw + 1); k++)
				{	
					compute_histogram(context, i + ref_image[k], tile_hist);
					out_pos++;
					out[out_pos] = compute_hist_dist(context, tile_hist); 
				}
			}
				


            for (i = 0; i < out_size; i++) {
                lide_c_fifo_write(context->out, &(out[i]));
            }
			free(tile_hist);
			free(*ref_image);
			free(ref_image);
			free(out);	
				
            context->mode = lide_c_objtr_hist_dist_map_MODE_1;
            break; 
        default:
            context->mode = lide_c_objtr_hist_dist_map_MODE_1;
            break;
    } 
    return;
}

void lide_c_objtr_hist_dist_map_terminate(
        lide_c_objtr_hist_dist_map_context_type *context) {
    free(context);
}
