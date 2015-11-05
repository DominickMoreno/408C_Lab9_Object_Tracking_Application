#ifndef _lide_c_hist_find_h
#define _lide_c_hist_find_h

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
#include "lide_c_actor.h"
#include "lide_c_fifo.h"

/*******************************************************************************
This actor consumes vector length (the number of elements in each vector)
values and vectors on separate inputs and computes the inner products of
corresponding pairs of input vectors. There are two modes associated with
this actor. In the STORE_LENGTH mode, the actor first consumes a vector length.
If this vector length is less than or equal to zero, then the next mode is set
to STORE_LENGTH (i.e., the same as the current mode), the actor does nothing
further in the current firing.  Otherwise (if the consumed vector length is
positive), the actor sets the length variable (internal actor parameter), and
sets the PROCESS mode as the next mode. In the PROCESS mode, the actor consumes
vectors based on the configured vector length, computes the inner product, and
sets the STORE_LENGTH mode as the next mode.
*******************************************************************************/

/* Actor modes */
/* Kim's actors */
/*
#define lide_c_objtr_hist_gen_MODE_STORE_IMAGE   1
#define lide_c_objtr_hist_gen_MODE_PROCESS       2 
*/

#define lide_c_hist_find_opt_MODE_PROCESS 99

/*******************************************************************************
TYPE DEFINITIONS
*******************************************************************************/

/* Structure and pointer types associated with inner product objects. */
/* kim's version */
/*
struct _lide_c_objtr_hist_gen_context_struct;
typedef struct _lide_c_objtr_hist_gen_context_struct
        lide_c_objtr_hist_gen_context_type;
*/

struct _lide_c_hist_find_opt_context_struct;
typedef struct _lide_c_hist_find_opt_context_struct
        lide_c_hist_find_opt_context_type;
/*******************************************************************************
INTERFACE FUNCTIONS
*******************************************************************************/

/*****************************************************************************
Construct function of the lide_c_objtr_hist_gen actor. Create a new
lide_c_objtr_hist_gen actor with the specified input FIFO pointer m for the
configuration of vector length, the specified input FIFO pointer x for the
first vector, the specified input FIFO pointer y for the second vector, and the
specified output FIFO pointer.
*****************************************************************************/
<<<<<<< HEAD:lab09/ota/src/objtr_hist_gen.h
objtr_hist_gen_context_type *objtr_hist_gen_new(
=======
/* kim's version */
/*
lide_c_objtr_hist_gen_context_type *lide_c_objtr_hist_gen_new(
>>>>>>> dominick-branch:lab09/ota/src/hist_find_opt/hist_find_opt.h
        lide_c_fifo_pointer input, int *bins, int NUM_BINS, int width,
        int height, lide_c_fifo_pointer out);
*/

lide_c_hist_find_opt_context_type *lide_c_hist_find_opt_new(
        lide_c_fifo_pointer input, int block_size, lide_c_fifo_pointer out);

/*****************************************************************************
Enable function of the lide_c_objtr_hist_gen actor.
*****************************************************************************/
/* Kim's version */
/*
boolean lide_c_objtr_hist_gen_enable(lide_c_objtr_hist_gen_context_type *context);
*/
boolean lide_c_hist_find_opt_enable(lide_c_hist_find_opt_context_type *context);

/*****************************************************************************
Invoke function of the lide_c_objtr_hist_gen actor.
*****************************************************************************/
void lide_c_hist_find_opt_invoke(lide_c_hist_find_opt_context_type *context);

/*****************************************************************************
Terminate function of the lide_c_objtr_hist_gen actor.
*****************************************************************************/
void lide_c_hist_find_opt_terminate(lide_c_hist_find_opt_context_type *context);

#endif
