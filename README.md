## Localthresh

Implementation of some fast local adaptive thresholding algorithms in C.

This program makes use of the GraphicsMagick library, available at http://www.graphicsmagick.org/

## Compilation

From the project directory, type "make".

## Usage

You can use one of the following syntax:

	./localthresh Mean [-w windowsize] [-c constant] input output
	./localthresh Niblack [-w windowsize] [-c constant] [-k std_dev_constant] input output

## References

More information about local thresholding based on the mean is available at http://homepages.inf.ed.ac.uk/rbf/HIPR2/adpthrsh.htm

Niblack's method is based on the book published by the same author: "An Introduction to Digital Image Processing" (1986)

## Licensing

This program is free software governed by the GNU General Public License (GPL). It is also distributed "as is", without warranties of any kind.
