// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
//  vi:set ts=4 sts=4 sw=4 noet :
//
// DISTANCE
// Copyright (C) 2015       Jakob Truelsen
// Copyright (C) 1997-1998  Gino van den Bergen
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free
// Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "distance.h"

double rel_error = 1e-6; // relative error in the computed distance
double abs_error = 1e-10; // absolute error if the distance is almost zero

static Point p[4];    // support points of object A in local coordinates 
static Point q[4];    // support points of object B in local coordinates 
static Point y[4];   // support points of A - B in world coordinates

static int bits;      // identifies current simplex
static int last;      // identifies last found support point
static int last_bit;  // last_bit = 1<<last
static int all_bits;  // all_bits = bits|last_bit 

static double det[16][4]; // cached sub-determinants

void compute_det() {
	static double dp[4][4];

	for (int i = 0, bit = 1; i < 4; ++i, bit <<=1) 
		if (bits & bit) dp[i][last] = dp[last][i] = dot(y[i], y[last]);
	dp[last][last] = dot(y[last], y[last]);

	det[last_bit][last] = 1;
	for (int j = 0, sj = 1; j < 4; ++j, sj <<= 1) {
		if (bits & sj) {
			int s2 = sj|last_bit;
			det[s2][j] = dp[last][last] - dp[last][j]; 
			det[s2][last] = dp[j][j] - dp[j][last];
			for (int k = 0, sk = 1; k < j; ++k, sk <<= 1) {
				if (bits & sk) {
					int s3 = sk|s2;
					det[s3][k] = det[s2][j] * (dp[j][j] - dp[j][k]) + 
						det[s2][last] * (dp[last][j] - dp[last][k]);
					det[s3][j] = det[sk|last_bit][k] * (dp[k][k] - dp[k][j]) + 
						det[sk|last_bit][last] * (dp[last][k] - dp[last][j]);
					det[s3][last] = det[sk|sj][k] * (dp[k][k] - dp[k][last]) + 
						det[sk|sj][j] * (dp[j][k] - dp[j][last]);
				}
			}
		}
	}
	if (all_bits == 15) {
		det[15][0] = det[14][1] * (dp[1][1] - dp[1][0]) + 
			det[14][2] * (dp[2][1] - dp[2][0]) + 
			det[14][3] * (dp[3][1] - dp[3][0]);
		det[15][1] = det[13][0] * (dp[0][0] - dp[0][1]) + 
			det[13][2] * (dp[2][0] - dp[2][1]) + 
			det[13][3] * (dp[3][0] - dp[3][1]);
		det[15][2] = det[11][0] * (dp[0][0] - dp[0][2]) + 
			det[11][1] * (dp[1][0] - dp[1][2]) +  
			det[11][3] * (dp[3][0] - dp[3][2]);
		det[15][3] = det[7][0] * (dp[0][0] - dp[0][3]) + 
			det[7][1] * (dp[1][0] - dp[1][3]) + 
			det[7][2] * (dp[2][0] - dp[2][3]);
	}
}

inline bool valid(int s) {  
	for (int i = 0, bit = 1; i < 4; ++i, bit <<= 1) {
		if (all_bits & bit) {
			if (s & bit) { if (det[s][i] <= 0) return false; }
			else if (det[s|bit][i] > 0) return false;
		}
	}
	return true;
}

inline void compute_vector(int bits1, Point& v) {
	double sum = 0;
	v.zero();
	for (int i = 0, bit = 1; i < 4; ++i, bit <<= 1) {
		if (bits1 & bit) {
			sum += det[bits1][i];
			v += y[i] * det[bits1][i];
		}
	}
	v *= 1 / sum;
}

inline void compute_points(int bits1, Point& p1, Point& p2) {
	double sum = 0;
	p1.zero();
	p2.zero();
	for (int i = 0, bit = 1; i < 4; ++i, bit <<= 1) {
		if (bits1 & bit) {
			sum += det[bits1][i];
			p1 += p[i] * det[bits1][i];
			p2 += q[i] * det[bits1][i];
		}
	}
	double s = 1 / sum;
	p1 *= s;
	p2 *= s;
}

inline bool closest(Point& v) {
	compute_det();
	for (int s = bits; s; --s) {
		if ((s & bits) == s) {
			if (valid(s|last_bit)) {
				bits = s|last_bit;
				compute_vector(bits, v);
				return true;
			}
		}
	}
	if (valid(last_bit)) {
		bits = last_bit;
		v = y[last];
		return true;
	}
	return false;
}

// The next function is used for detecting degenerate cases that cause 
// termination problems due to rounding errors.  
   
inline bool degenerate(const Point& w) {
	for (int i = 0, bit = 1; i < 4; ++i, bit <<= 1) 
		if ((all_bits & bit) && y[i] == w)  return true;
	return false;
}

void closest_points(const Polygon & a, const Polygon & b,
					Point& pa, Point& pb) {
	Point v = a.points[0] - b.points[0];
	double dist = v.length();
	Point w;
	bits = 0;
	all_bits = 0;
	double mu = 0;
	while (bits < 15 && dist > abs_error) {
		last = 0;
		last_bit = 1;
		while (bits & last_bit) { ++last; last_bit <<= 1; }
		p[last] = a.support(-v);
		q[last] = b.support(v);
		w = p[last] - q[last];
		mu = std::max(mu, dot(v, w) / dist);
		if (dist - mu <= dist * rel_error) break; 
		if (degenerate(w)) {
			break;
		}
		y[last] = w;
		all_bits = bits|last_bit;
		if (!closest(v)) {
			break;
		}
		dist = v.length();
	}
	compute_points(bits, pa, pb);
}



