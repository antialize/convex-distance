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

#ifndef _DISTANCE_H_
#define _DISTANCE_H_

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

class Point {
public:
	Point() {}
	Point(const double v[2]) : comp{v[0], v[1]} {}
	Point(double x, double y) : comp{x, y} {}
	
	double&       operator[](int i)       { return comp[i]; }
	const double& operator[](int i) const { return comp[i]; }
	
	Point& operator+=(const Point& v) {
		comp[0] += v[0]; comp[1] += v[1];
		return *this;
	}
	
	Point& operator-=(const Point& v) {
		comp[0] -= v[0]; comp[1] -= v[1];
		return *this;
	}
	
	Point& operator*=(double s) {
		comp[0] *= s; comp[1] *= s;
		return *this;
	}
	
	Point& operator/=(double s) {return *this *= 1 / s;}
	void zero() {comp[0] = 0; comp[1] = 0;}
	double length2() const {return dot(*this, *this); }
	double length() const {return sqrt(length2()); }
	
	friend Point operator+(const Point& v1, const Point& v2) {return Point(v1[0] + v2[0], v1[1] + v2[1]); }
	friend Point operator-(const Point& v1, const Point& v2) {return Point(v1[0] - v2[0], v1[1] - v2[1]); }
	friend Point operator-(const Point& v) {return Point(-v[0], -v[1]);}
	friend Point operator*(const Point& v, double s) {return Point(v[0] * s, v[1] * s);}
	friend Point operator*(double s, const Point& v) {return v * s; }
	friend double dot(const Point& v1, const Point& v2) {return v1[0] * v2[0] + v1[1] * v2[1]; }
	friend double length2(const Point& v) {return v.length2(); }
	friend double length(const Point& v) {return v.length(); }
	friend bool operator==(const Point& t1, const Point& t2) {return t1[0] == t2[0] && t1[1] == t2[1]; }
	friend std::ostream& operator<<(std::ostream& os, const Point& t) {return os << t[0] << ' ' << t[1]; }
  
protected:
	double comp[2];
};


typedef std::vector<Point> Polygon;

// class Polygon {
// public:
// 	Polygon( std::vector<Point> points): points(points), ci(0) {}
 
// 	Point support(const Point & v) const {
// 		double h = dot(points[ci], v), d;
// 		int ni = ci < points.size()-1 ? ci+1 : 0;
// 		if ((d = dot(points[ni], v)) > h) { 
// 			do { 
// 				h = d; ci = ni;
// 				if (++ni == points.size()) ni = 0;
// 			}
// 			while ((d = dot(points[ni], v)) > h);
// 		}
// 		else {
// 			ni = ci ? ci-1 : points.size()-1;
// 			while ((d = dot(points[ni], v)) > h) {
// 				h = d; ci = ni;
// 				if (ni) --ni; else ni = points.size()-1;
// 			}
// 		}  
// 		return points[ci];
// 	}
	
// 	std::vector<Point> points;
// 	mutable int ci;
// };

void closest_points(const Polygon &, const Polygon &, Point&, Point&);

#endif // _DISTANCE_H_
