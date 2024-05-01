#ifndef INTERSECT_H
#define INTERSECT_H

#include <vector>
#include <queue>
#include <set>
#include <stack>
#include <algorithm>

enum PointType { LEFT, RIGHT, TOP, BOTTOM };

enum LineType { HORIZONTAL, VERTICAL };

struct Point {
    double x, y;
    PointType type;
    int line_index;

    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    bool operator < (const Point p2) const {
        return x <= p2.x;
    }
};

struct Line {
    LineType type;
    Line() {};
    Line(Point p1, Point p2) {
        type = p1.x == p2.x ? VERTICAL : HORIZONTAL;

        if (p1.x < p2.x || p1.y < p2.y) {
            first_point = p1;
            second_point = p2;
        } else {
            first_point = p2;
            second_point = p1;
        }
        first_point.type = LEFT;
        second_point.type = RIGHT;

        if (p1.y < p2.y){
            _first_point = p1;
            _second_point = p2;
        } else {
            _first_point = p2;
            _second_point = p1;
        }
        _first_point.type = TOP;
        _second_point.type = BOTTOM;
    }

    Point first_point;
    Point second_point;
    Point _first_point;
    Point _second_point;

    bool operator < (const Line& s1) const {
        return _second_point.y > s1._second_point.y;
    }
};

std::vector<std::pair<int, int>> ComputeIntersections(std::vector<Line> lines);

#endif // INTERSECT_H
