#ifndef GRAHAMHULL_H
#define GRAHAMHULL_H

#include <QPainter>
#include <vector>

struct point {
    double x;
    double y;

    double* to_double_ptr() {
        return reinterpret_cast<double*>(this);
    }
};

class GrahamHull
{
    std::vector<point> points;
    std::vector<int> border;

public:
    GrahamHull(std::vector<point> const& points);
    void ComputeGrahamHull();
    bool ComparePolar(point &p1, point &p2);

    const std::vector<point>& GetPoints() {
        return points;
    }

    const std::vector<int>& GetBorder() {
        ComputeGrahamHull();
        return border;
    }
};

#endif // GRAHAMHULL_H
