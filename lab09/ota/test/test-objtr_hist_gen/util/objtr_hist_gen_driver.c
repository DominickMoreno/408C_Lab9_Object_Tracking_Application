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
#include "lide_c_file_source.h"
#include "lide_c_file_sink.h"
#include "objtr_hist_gen.h"
#include "lide_c_util.h"

//#define BUFFER_CAPACITY 1024

/* An enumeration of the actors in this application. */
#define ACTOR_INPUTSOURCE 0
#define ACTOR_OBJTR_HIST_GEN 1
#define ACTOR_SINK 2

/* The total number of actors in the application. */
#define ACTOR_COUNT 3

#define NUM_BINS 2
#define WIDTH 2
#define HEIGHT 2

/* 
    Usage: lide_c_inner_prod_driver.exe m_file x_file y_file out_file
*/
int main(int argc, char **argv) {
    FILE *input_file = NULL; 
    FILE *out_file = NULL; 
    objtr_hist_gen_context_type *actors[ACTOR_COUNT];
    
    /* Connectivity: fifo1: (m, product), fifo2: (x, product); 
       fifo3: (y, product); fifo4: (product, out) 
    */
    lide_c_fifo_pointer fifo1 = NULL, fifo2 = NULL;

    int token_size = 0;
    int bins[NUM_BINS * 2] = {0, 199, 200, 400};
    int i = 0;
    int arg_count = 2;
    int buffer_capacity = WIDTH * HEIGHT;
    
    /* actor descriptors (for diagnostic output) */
    char *descriptors[ACTOR_COUNT] = {"input_source",  
            "objtr_hist_gen", "sink"};

    /* Check program usage. */
    if (argc != arg_count) {
        fprintf(stderr, "objtr_hist_gen_driver.exe error: arg count (argc = %d", argc);
        exit(1);
    }   

    /* Open the input and output file(s). */
    i = 1;
    input_file = lide_c_util_fopen(argv[i++], "r"); 
    out_file = lide_c_util_fopen(argv[i++], "w"); 

    /* Create the buffers. */
    token_size = sizeof(int);
    fifo1 = lide_c_fifo_new(buffer_capacity, token_size);
    fifo2 = lide_c_fifo_new(buffer_capacity, token_size);

    /* Create and connect the actors. */
    i = 0;
    actors[ACTOR_INPUTSOURCE] = (lide_c_actor_context_type
            *)(lide_c_file_source_new(input_file, fifo1));

    actors[ACTOR_OBJTR_HIST_GEN] = (lide_c_actor_context_type
            *)(objtr_hist_gen_new(fifo1, &bins, NUM_BINS, WIDTH, HEIGHT, fifo2));

    actors[ACTOR_SINK] = (lide_c_actor_context_type *)
            (lide_c_file_sink_new(out_file, fifo2));

    /* Execute the graph. */
    lide_c_util_simple_scheduler(actors, ACTOR_COUNT, descriptors);

    /* Normal termination. */
    return 0;
}