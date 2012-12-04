/* 
 * Algorithms.c - Implementation of some efficient local thresholding algorithms 
 */

/* This file is part of localthresh
 * 
 * localthresh is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
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
#include "algorithms.h"
#include "misc.h"

Image *
mean_local_threshold(const Image *in_img, const int win_size, const int c)
{
    unsigned int num_rows = in_img->rows;
    unsigned int num_cols = in_img->columns;
    int ir, ic;
    Image *bin_img;
    unsigned char *in_data, *bin_data;
    unsigned long long int *int_data;
    ExceptionInfo exception;
    int half_ws = win_size >> 1;
    double mean;

    /* Precompute an integral image to accelerate computations */
    GetExceptionInfo(&exception);
    in_data = (unsigned char*)malloc(num_rows * num_cols * sizeof(unsigned char));
    if (MagickFail == DispatchImage(in_img, 0, 0, num_cols, num_rows, "I", CharPixel, in_data, &exception)) {
	fprintf(stderr, "There was a problem extracting image pixels. Exception=%s\n", exception.description);
	exit(EXIT_FAILURE);
    }

    int_data = compute_integral_image(in_img, 0);
    if (NULL == int_data) {
	fprintf(stderr, "Insufficient memory\n");
	exit(EXIT_FAILURE);
    }
  
    bin_data = (unsigned char*)malloc(num_cols * num_rows * sizeof(unsigned char));

    for (ir = 0; ir < num_rows; ir++)
	for (ic = 0; ic < num_cols; ic++) {
	    if (ir+half_ws >= num_rows ||
		ic+half_ws >= num_cols ||
		ir-half_ws <= 0 ||
		ic-half_ws <= 0) mean = 0.0;
	    else {
		mean = (int_data[(ir+half_ws)*num_cols+ic+half_ws] + 
			int_data[(ir-half_ws)*num_cols+ic-half_ws] - 
			int_data[(ir-half_ws)*num_cols+ic+half_ws] - 
			int_data[(ir+half_ws)*num_cols+ic-half_ws]) / (double)(win_size * win_size);
	    }

	    bin_data[ir*num_cols+ic] = ((in_data[ir*num_cols+ic] > mean - c) ? OBJECT : BACKGROUND);
	}

    bin_img = ConstituteImage(num_cols, num_rows, "I", CharPixel, bin_data, &exception);

    free(in_data);
    free(bin_data);
    DestroyExceptionInfo(&exception);

    return bin_img;
}

Image *
niblack_local_threshold(const Image *in_img, const int win_size, const double k, const int c)
{
    unsigned int num_rows = in_img->rows;
    unsigned int num_cols = in_img->columns;
    int ir, ic;
    Image *bin_img;
    unsigned char *in_data, *bin_data;
    unsigned long long int *int_data, *int_squared_data;
    ExceptionInfo exception;
    int half_ws = win_size >> 1;
    double mean_of_squares, mean;

    /* Precompute an integral image to accelerate computations */
    GetExceptionInfo(&exception);
    in_data = (unsigned char*)malloc(num_rows * num_cols * sizeof(unsigned char));
    if (MagickFail == DispatchImage(in_img, 0, 0, num_cols, num_rows, "I", CharPixel, in_data, &exception)) {
	fprintf(stderr, "There was a problem extracting image pixels. Exception=%s\n", exception.description);
	exit(EXIT_FAILURE);
    }

    int_data = compute_integral_image(in_img, 0);
    if (NULL == int_data) {
	fprintf(stderr, "Insufficient memory\n");
	exit(EXIT_FAILURE);
    }

    int_squared_data = compute_integral_image(in_img, 1);
    if (NULL == int_squared_data) {
	fprintf(stderr, "Insufficient memory\n");
	exit(EXIT_FAILURE);
    }
  
    bin_data = (unsigned char*)malloc(num_cols * num_rows * sizeof(unsigned char));

    for (ir = 0; ir < num_rows; ir++)
	for (ic = 0; ic < num_cols; ic++) {
	    if (ir+half_ws >= num_rows ||
		ic+half_ws >= num_cols ||
		ir-half_ws <= 0 ||
		ic-half_ws <= 0) {
		mean = 0.0;
		mean_of_squares = 0.0;
	    }
	    else {
		mean_of_squares = (int_squared_data[(ir+half_ws)*num_cols+ic+half_ws] + 
				   int_squared_data[(ir-half_ws)*num_cols+ic-half_ws] - 
				   int_squared_data[(ir-half_ws)*num_cols+ic+half_ws] - 
				   int_squared_data[(ir+half_ws)*num_cols+ic-half_ws]) / (double)(win_size * win_size);
		mean = (int_data[(ir+half_ws)*num_cols+ic+half_ws] + 
			int_data[(ir-half_ws)*num_cols+ic-half_ws] -
			int_data[(ir-half_ws)*num_cols+ic+half_ws] - 
			int_data[(ir+half_ws)*num_cols+ic-half_ws]) / (double)(win_size * win_size);
	    }

	    if (in_data[ir*num_cols+ic] > (mean + k * sqrt(mean_of_squares - mean * mean) - c))
		bin_data[ir*num_cols+ic] = OBJECT;
	    else
		bin_data[ir*num_cols+ic] = BACKGROUND;
	}

    bin_img = ConstituteImage(num_cols, num_rows, "I", CharPixel, bin_data, &exception);

    free(in_data);
    free(bin_data);
    DestroyExceptionInfo(&exception);

    return bin_img;
}
