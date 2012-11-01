/* 
 * Misc.c - Miscellaneous helper functions
 */

/* This file is part of localthresh
 * 
 * localthresh is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 * 
 * localthresh is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * with localthresh.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author: Daniel Martín 
 */

#include "main.h"
#include "misc.h"

unsigned long *
compute_integral_image(const Image *in_img)
{
    unsigned int num_rows = in_img->rows;
    unsigned int num_cols = in_img->columns;
    unsigned long *out_data;
    Image *out_img;
    unsigned char *in_data;
    int ir, ic;
    ExceptionInfo exception;

    in_data = (unsigned char*)malloc(num_cols * num_rows * sizeof(unsigned char));

    GetExceptionInfo(&exception);
    if (MagickFail == DispatchImage(in_img, 0, 0, num_cols, num_rows, "I", CharPixel, in_data, &exception)) {
	fprintf(stderr, "There was a problem extracting image pixels. Exception=%s\n", exception.description);
	exit(EXIT_FAILURE);
    }

    /* Allocate space for the output image */
    out_data = (unsigned long*)malloc(num_cols * num_rows * sizeof(unsigned long));

    for (ir = 0; ir < num_rows; ir++)
	for (ic = 0; ic < num_cols; ic++) {				      
	    if (ir == 0 && ic == 0) out_data[0] = in_data[0];
	    else if (ir == 0) out_data[ic] = in_data[ic] + out_data[ic-1];
	    else if (ic == 0) out_data[ir*num_cols] = in_data[ir*num_cols] + out_data[(ir-1)*num_cols];
	    else out_data[ir*num_cols+ic] = in_data[ir*num_cols+ic] + out_data[(ir-1)*num_cols+ic] + out_data[ir*num_cols+ic-1] - out_data[(ir-1)*num_cols+ic-1];
	}

    DestroyExceptionInfo(&exception);
    free(in_data);

    return out_data;
}
