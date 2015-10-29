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
#include "objtr_hist_gen.h"
#include "lide_c_util.h"

/*******************************************************************************
INNER PRODUCT STRUCTURE DEFINITION
*******************************************************************************/

struct _objtr_hist_gen_context_struct { 
#include "lide_c_actor_context_type_common.h"

    /* Histogram variables. */
    int num_bins;
    int *bins;
    int width; 
    int height;
    int *image;
     

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
objtr_hist_gen_context_type *objtr_hist_gen_new(
        lide_c_fifo_pointer input, 
        int *bins, 
        int num_bins,
        int width,
        int height,
        lide_c_fifo_pointer out) { 

    objtr_hist_gen_context_type *context = NULL;

    context = lide_c_util_malloc(
            sizeof(objtr_hist_gen_context_type));
    context->mode = 
            OBJTR_HIST_GEN_MODE_STORE_IMAGE;
    context->enable = (lide_c_actor_enable_function_type)
            objtr_hist_gen_enable;
    context->invoke = (lide_c_actor_invoke_function_type)
            objtr_hist_gen_invoke;
    context->bins = bins;
    context->num_bins = num_bins;
    context->width = width;
    context->height = height;
    context->image = NULL;
    context->out = out;
    return context;
}

boolean objtr_hist_gen_enable(
        objtr_hist_gen_context_type *context) {
    boolean result = FALSE;

    switch (context->mode) {
    case OBJTR_HIST_GEN_MODE_STORE_IMAGE:
        result = lide_c_fifo_population(context->input) == ((context->width)*(context->height));
        break;
    case OBJTR_HIST_GEN_MODE_PROCESS:
        result = (lide_c_fifo_population(context->out) <
                lide_c_fifo_capacity(context->out));
        break;
    default:
            result = FALSE;
            break;
    }
    return result;
}

void objtr_hist_gen_invoke(objtr_hist_gen_context_type *context) {
    int i = 0;
    int num_pixels = context->width * context->height;
    int *temp = context->image;
    switch (context->mode) {
    case OBJTR_HIST_GEN_MODE_STORE_IMAGE:
        for (i = 0; i < num_pixels; i++) {  
            lide_c_fifo_read(context->input, &temp);
            temp++;
        }
        

        /* Disregard this token if it results in an invalid length. 
         if (context->length <= 0) {
            context->mode = LIDE_C_INNER_PROD_MODE_STORE_LENGTH;
             return;
         } */
        context->mode = OBJTR_HIST_GEN_MODE_PROCESS;
        break;

    case OBJTR_HIST_GEN_MODE_PROCESS:
        for (i = 0; i < num_pixels; i++) {
            lide_c_fifo_write(context->out, context->image[i]);
        }
        context->mode = OBJTR_HIST_GEN_MODE_STORE_IMAGE;
        break; 
    default:
        context->mode = OBJTR_HIST_GEN_MODE_STORE_IMAGE;
        break;
    } 
    return;
}

void objtr_hist_gen_terminate(
        objtr_hist_gen_context_type *context) {
    free(context);
}
