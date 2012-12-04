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

#ifndef __ALGORITHMS_H_
#define __ALGORITHMS_H_

Image *
mean_local_threshold(const Image *in_img, const int win_size, const int c);
Image *
niblack_local_threshold(const Image *in_img, const int win_size, const double k, const int c);

#endif
