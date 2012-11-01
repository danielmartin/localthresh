/* 
 * Main.c - Entry point for localthresh 
 * 
 * This program contains the implementation of some improved local thresholding algorithms.
 * They make use of dynamic programming to reduce the traditional
 * running time of this kind of algorithms from O(W^2*N^2) to O(N^2) by using additional space.
 * 
 * It makes use of the GraphicsMagick C API
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

void 
print_usage()
{
    fprintf(stderr, "Usage: localthresh input_image output_image [-w window_size] [-c constant]\n");
    exit(1);
}

int 
main(int argc, char **argv)
{
    int index;
    Image *in_img, *out_img;
    ExceptionInfo exception;
    ImageInfo *in_img_info, *out_img_info;
    clock_t start_time;
    double elapsed_time;
    char out_filename[MAX_FILENAME];
    size_t in_img_info_filename_length, out_img_filename_length;
    long win_size, c;
    char *endptr;
    char *char_win_size, *char_c, *input_file, *output_file;
    char_win_size = char_c = input_file = output_file = NULL;

    if (argc < 3) {
	fprintf(stderr, "Invalid number of arguments\n");
	print_usage();
	exit(EXIT_FAILURE);
    }

    opterr = 0;
    while ((c = getopt(argc, argv, "w:c:")) != -1)
	switch (c) {
	case 'w':
	    char_win_size = optarg;
	    break;
	case 'c':
	    char_c = optarg;
	    break;
	case '?':
	    if (optopt == 'w' || optopt == 'c')
		fprintf(stderr, "Option -%c requires a numeric argument.\n", optopt);
	    else if (isprint (optopt))
		fprintf(stderr, "Unknown option `-%c'.\n", optopt);
	    else
		fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
	    print_usage();
	    exit(EXIT_FAILURE);
	default:
	    print_usage();
	}
    
    win_size = strtol(char_win_size, &endptr, 10);
    if (endptr == char_win_size) {
	fprintf(stderr, "Window size value is incorrect.\n");
	print_usage();
	exit(EXIT_FAILURE);
    }

    c = strtol(char_c, &endptr, 10);
    if (endptr == char_c) {
	fprintf(stderr, "Constant is incorrect.\n");
	print_usage();
	exit(EXIT_FAILURE);
    }

    if ((errno == ERANGE && (win_size == LONG_MAX || win_size == LONG_MIN))
	|| (errno == ERANGE && (c == LONG_MAX || c == LONG_MIN))
	|| (errno != 0 && win_size == 0)
	|| (errno != 0 && c == 0)) {
	perror("strtol");
	exit(EXIT_FAILURE);
    }

    if (argc - optind != 2) {
	fprintf(stderr, "Input and output files are not supplied.\n");
	print_usage();
	exit(EXIT_FAILURE);
    }
    /* First argument is input file */
    input_file = argv[optind];

    /* Second argument is output file */
    output_file = argv[optind+1];

    InitializeMagick(*argv);
    GetExceptionInfo(&exception);
    in_img_info = CloneImageInfo((ImageInfo*)NULL);
    in_img_info_filename_length = MaxTextExtent;
    strncpy(in_img_info->filename, input_file, in_img_info_filename_length);
    if (in_img_info_filename_length) {
	in_img_info->filename[in_img_info_filename_length - 1] = '\0';
    }

    /* Read the input image */
    in_img = ReadImage(in_img_info, &exception);
    if (NULL == in_img) {
	fprintf(stderr, "An error ocurred while reading the input image: %s\n", exception.description);
	exit(EXIT_FAILURE);
    }
  
    if (!IsGrayImage(in_img, &exception)) {
	fprintf(stderr, "Input image %s must be grayscale\n", input_file);
	exit(EXIT_FAILURE);
    }

    start_time = clock();
    out_img = mean_local_threshold(in_img, win_size, c);
    elapsed_time = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;
    printf("Time mean improved: %f\n", elapsed_time);

    /* Write the binarized image to disk */
    out_img_info = CloneImageInfo((ImageInfo*)NULL);
    out_img_filename_length = MaxTextExtent;
    strncpy(out_img->filename, output_file, out_img_filename_length);
    if (out_img_filename_length)
	out_img->filename[out_img_filename_length - 1] = '\0';

    unsigned int result = WriteImage(out_img_info, out_img);
    if (!result) {
	fprintf(stderr, "Cannot write image to disk\n");
	exit(EXIT_FAILURE);
    }

    printf("The image was successfully binarized.\n");

    /* Clear memory */
    DestroyImageInfo(in_img_info);
    DestroyImageInfo(out_img_info);
    DestroyImage(in_img);
    DestroyImage(out_img);
    DestroyExceptionInfo(&exception);
    DestroyMagick();

    return(0);
}
