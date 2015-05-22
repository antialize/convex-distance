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

int main() {
	{
		Polygon a {Point(0, 0), Point(0,9), Point(3,3), Point(0,3)};
		Polygon b {Point(4, 4), Point(4,6), Point(6,4)};
		
		Point p_a;
		Point p_b;
		closest_points(a, b, p_a, p_b);
		std::cout << p_a << " bs " << p_b << std::endl;
	}
	{
		Polygon a {{Point(0, 0), Point(0,6), Point(6,6), Point(6,0)}};
		Polygon b {{Point(4, 4), Point(5,5)}};
		
		Point p_a;
		Point p_b;
		closest_points(a, b, p_a, p_b);
		std::cout << p_a << " bs " << p_b << std::endl;
	}
}
