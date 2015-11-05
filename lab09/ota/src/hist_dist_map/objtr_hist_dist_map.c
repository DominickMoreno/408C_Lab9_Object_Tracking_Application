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
#include "lide_c_objtr_hist_dist_map.h"
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
    lide_c_fifo_pointer input;

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
	/*FIXME: Maybe malloc space for histgram?*/
    context->histogram = histogram;
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
    boolean result = FALSE;
    switch (context->mode) {
        case lide_c_objtr_hist_dist_map_MODE_1:
            result = lide_c_fifo_population(context->hist) ==
				context->bin_count;
            break;
        case lide_c_objtr_hist_dist_map_MODE_2:
		/*FIXME: Not sure if output is correct*/ 
            result = (lide_c_fifo_population(context->out) <
                    lide_c_fifo_capacity(context->out));
            break;
        default:
                result = FALSE;
                break;
    }
    return result;
}

void lide_c_objtr_hist_dist_map_invoke(lide_c_objtr_hist_dist_map_context_type *context) {
    int i, j, k, upper_bound, lower_bound = 0;
    int m = context->num_bins;
    int width = context->width;
    int height = context->height;
    int num_pixels = width * height;
    int *image;
    int *out;
    printf("in invoke\n");
    switch (context->mode) {
        case lide_c_objtr_hist_dist_map_MODE_1:
			/* 
            /* Disregard this token if it results in an invalid length. */
             if ((context->width <= 0) || (context->height <= 0)) {
                context->mode = lide_c_objtr_hist_dist_map_MODE_STORE_IMAGE;
                 return;
             } 
            context->mode = lide_c_objtr_hist_dist_map_MODE_2;
            break;

        case lide_c_objtr_hist_dist_map_MODE_2:
            for (i = 0; i < num_pixels; i++) {
                lide_c_fifo_read(context->input, &(context->image[i]));
            }

            out = lide_c_util_malloc(sizeof(int) * context->num_bins);
            image = context->image;


            for(i = 0; i < context->height; i++) {
                for(j = 0; j < context->width; j++) {
                    for(k = 0; k < 2*m; k += 2) {
                       
                        lower_bound = context->bins[k];
                        upper_bound = context->bins[k + 1];

                        if(((k == 0) && (image[i*width + j] < lower_bound)) ||
                           ((k == ((2*m)-2)) && (image[i*width + j] > upper_bound))) {
                            printf("Value %d out of range.\n", image[i + j]);
                            break;
                        }

                        if((image[i*width + j] >= lower_bound) &&
                           (image[i*width + j] <= upper_bound)) {
                            printf("%d is between %d and %d, adding to bin.\n", image[i*width + j], lower_bound, upper_bound);
                            out[(k/2)]++;
                            break;
                        }
                    }
                }   
            }

            for (i = 0; i < m; i++) {
                lide_c_fifo_write(context->out, &(out[i]));
            }
            free(out);
            context->mode = lide_c_objtr_hist_dist_map_MODE_1;
            break; 
        default:
            context->mode = lide_c_objtr_hist_dist_map_MODE_1_
            break;
    } 
    return;
}

void lide_c_objtr_hist_dist_map_terminate(
        lide_c_objtr_hist_dist_map_context_type *context) {
    free(context->image);
    free(context);
}
