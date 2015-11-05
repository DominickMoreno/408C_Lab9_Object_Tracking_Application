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

    /* Input port. */
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
lide_c_hist_find_opt_context_type *lide_c_hist_find_opt_new(
        lide_c_fifo_pointer input, 
        int block_size,
        lide_c_fifo_pointer out) { 
	printf("in hist_find_opt_NEW!\n");
    return NULL;
}

boolean lide_c_hist_find_opt_enable(
        lide_c_hist_find_opt_context_type *context) {

	printf("in hist_find_opt_ENABLE!\n");
	return 0;
}

void lide_c_hist_find_opt_invoke(lide_c_hist_find_opt_context_type *context) {
    printf("in hist_find_opt_INVOKE!\n");
    return;
}

void lide_c_hist_find_opt_terminate(
        lide_c_hist_find_opt_context_type *context) {

	printf("in hist_find_opt_TERMINATE!\n");
	return;
}
