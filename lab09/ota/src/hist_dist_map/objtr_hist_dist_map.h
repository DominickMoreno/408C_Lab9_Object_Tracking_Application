#ifndef _lide_c_objtr_hist_dist_map_h
#define _lide_c_objtr_hist_dist_map_h

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
#define lide_c_objtr_hist_dist_map_MODE_1   1
#define lide_c_objtr_hist_dist_map_MODE_2   2 

/*******************************************************************************
TYPE DEFINITIONS
*******************************************************************************/

/* Structure and pointer types associated with inner product objects. */
struct _lide_c_objtr_hist_dist_map_context_struct;
typedef struct _lide_c_objtr_hist_dist_map_context_struct
        lide_c_objtr_hist_dist_map_context_type;

/*******************************************************************************
INTERFACE FUNCTIONS
*******************************************************************************/

/*****************************************************************************
Construct function of the lide_c_objtr_hist_dist_map actor. Create a new
lide_c_objtr_hist_dist_map actor with the specified input FIFO pointer m for the
configuration of vector length, the specified input FIFO pointer x for the
first vector, the specified input FIFO pointer y for the second vector, and the
specified output FIFO pointer.
*****************************************************************************/

lide_c_objtr_hist_dist_map_context_type *lide_c_objtr_hist_dist_map_new(
        lide_c_fifo_pointer ref, lide_c_fifo_pointer hist, int *histogram,
		int *bins, int bin_count, int W, int H, int Tw, int Th, lide_c_fifo_pointer out); 
/*****************************************************************************
Enable function of the lide_c_objtr_hist_dist_map actor.
*****************************************************************************/
boolean lide_c_objtr_hist_dist_map_enable(lide_c_objtr_hist_dist_map_context_type *context);

/*****************************************************************************
Invoke function of the lide_c_objtr_hist_dist_map actor.
*****************************************************************************/
void lide_c_objtr_hist_dist_map_invoke(lide_c_objtr_hist_dist_map_context_type *context);

/*****************************************************************************
Terminate function of the lide_c_objtr_hist_dist_map actor.
*****************************************************************************/
void lide_c_objtr_hist_dist_map_terminate(lide_c_objtr_hist_dist_map_context_type *context);

/*****************************************************************************
Getter: histogram
*****************************************************************************/
int *histogram_context_get(lide_c_objtr_hist_dist_map_context_type *context);
#endif
