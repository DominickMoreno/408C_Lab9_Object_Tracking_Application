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
/* Kim's library */
/*
#include "lide_c_objtr_hist_gen.h"
*/
#include "hist_find_opt.h"
#include "lide_c_util.h"

/*******************************************************************************
INNER PRODUCT STRUCTURE DEFINITION
*******************************************************************************/

/* Kim's context obj */
/*
struct _lide_c_objtr_hist_gen_context_struct { 
#include "lide_c_actor_context_type_common.h"

    //Histogram variables
    int num_bins;
    int *bins;
    int width; 
    int height;
    int *image;
     

    //Input ports.
    lide_c_fifo_pointer input;

    //Output port.
    lide_c_fifo_pointer out;
};
*/

struct _lide_c_hist_find_opt_context_struct { 
#include "lide_c_actor_context_type_common.h"

    /* Histogram variables. */
    int block_size;
	int output_index;

    /* Input port. */
    lide_c_fifo_pointer input;

    /* Output port. */
    lide_c_fifo_pointer output;
};
/*******************************************************************************
IMPLEMENTATIONS OF INTERFACE FUNCTIONS.
*******************************************************************************/

/* This function code (and other code in this file) is formatted with extra 
line breaks so that it can be cut and pasted more conveniently into 
presentation slides. 
*/
lide_c_hist_find_opt_context_type *lide_c_hist_find_opt_new(
        lide_c_fifo_pointer input, 
        int block_size,
        lide_c_fifo_pointer output) { 
	lide_c_hist_find_opt_context_type *context = NULL;
	printf("in hist_find_opt_NEW!\n");

	/* allocate memory */
	context = lide_c_util_malloc(sizeof(lide_c_hist_find_opt_context_type));
	context->mode = lide_c_hist_find_opt_MODE_PROCESS;

	/* assign enable/invoke */
	context->enable = (lide_c_actor_enable_function_type)
		lide_c_hist_find_opt_enable;
	context->invoke = (lide_c_actor_invoke_function_type)
		lide_c_hist_find_opt_invoke;

	/* bind input/output FIFOs to context */
	context->input = input;
	context->output = output;

	/* assign actor specific context data */
	context->block_size = block_size;
	context->output_index = -1;

    return context;
}

boolean lide_c_hist_find_opt_enable(
        lide_c_hist_find_opt_context_type *context) {

	boolean result = 0;

	printf("in hist_find_opt_ENABLE!\n");
	if(context->mode == lide_c_hist_find_opt_MODE_PROCESS) {

		int num_tokens = lide_c_fifo_population(context->input);
		result = (num_tokens >= context->block_size);

	} 

	return result;
}

void lide_c_hist_find_opt_invoke(
		lide_c_hist_find_opt_context_type*context) {
    printf("in hist_find_opt_INVOKE!\n");

	if(context->mode == lide_c_hist_find_opt_MODE_PROCESS) {
		int index, least_val, least_index;
		int *array_of_vals =
			lide_c_util_malloc(sizeof(int) * context->block_size);

		/* read the input FIFO */
		for(index = 0; index < context->block_size; index++) {
			array_of_vals[index] = -1;
			lide_c_fifo_read(context->input, &array_of_vals[index]);
		} 

		/* determine the index of the smallest value */
		least_val = 2147483647;
		least_index = -1;
		for(index = 0; index < context->block_size; index++) {
			if(array_of_vals[index] < least_val) {
				least_val = array_of_vals[index];
				least_index = index;
			}
		}

		context->output_index = least_index;

		/* write the least value index to the output FIFO */
		lide_c_fifo_write(context->output, &(context->output_index));

		free(array_of_vals);

	}

    return;
}

void lide_c_hist_find_opt_terminate(
        lide_c_hist_find_opt_context_type *context) {

	printf("in hist_find_opt_TERMINATE!\n");
	free(context);
	return;
}
