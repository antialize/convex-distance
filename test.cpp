// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
//  vi:set ts=4 sts=4 sw=4 noet :
//
// DISTANCE
// Copyright (C) 2015       Jakob Truelsen
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
#include <string>
#include <iostream>

int test(Polygon a,
		 Polygon b,
		 double dist_sq) {
	Point p_a;
	Point p_b;
	closest_points(a, b, p_a, p_b);
	double ds=length2(p_a-p_b);
	if (fabs(ds - dist_sq) < 1e-6) return EXIT_SUCCESS;
	std::cerr << "Distance " << ds << " should be " << dist_sq
			  << " (" << p_a << " to " << p_b << ")" << std::endl;
	return EXIT_FAILURE;
}

int main(int argc, char ** argv) {
	const std::string t = argv[1];
	if (t == "lines1")
		return test({{1,1}, {1,3}}, {{2,2}, {3,2}}, 1.0);
	else if (t == "lines2")
		return test({{1,1}, {1,3}}, {{2,4}, {3,4}}, 2.0);
	else if (t == "lines3")
		return test({{1,1}, {3,3}}, {{1,3}, {3,1}}, 0.0);
	else if (t == "lines4")
		return test({{1,1}, {1,3}}, {{1,1}, {1,3}}, 0.0);
	else if (t == "lines5")
	 	return test({{0,0}, {0,6}, {6,6}, {6,0}}, {{4,4}, {5,5}}, 0.0);
	else if (t == "point1")
		return test({{1,1}}, {{1,3}, {3,1}}, 2.0);
	else if (t == "point2")
	 	return test({{1,1}}, {{1,1}, {2,2}}, 0.0);
	else if (t == "point3")
		return test({{1,1}}, {{2,2}}, 2.0);
	else if (t == "tri1")
	 	return test({{1,1}, {1,2}, {2,1}}, {{1,1}, {1,2}, {2,1}}, 0.0);
	else if (t == "tri2")
	 	return test({{1,1}, {1,2}, {2,1}}, {{2,2}, {1,2}, {2,1}}, 0.0);
	else if (t == "tri3")
	 	return test({{0,0}, {0,10}, {10,0}}, {{1,1}, {2,1}, {1,2}}, 0.0);
	else if (t == "gen1")
		return test({{0,0}, {0,9}, {3,3}, {0,3}}, {{4, 4}, {4,6}, {6,4}}, 1.8);
	else
		return EXIT_FAILURE;
}
