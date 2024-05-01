#ifndef NODE_H
#define NODE_H

#include <vector>

#include <QPoint>
#include <QRect>

class Node
{
private:
    QPoint point;
    bool horizontal;

    Node* left;
    Node* right;

    Node(std::vector<QPoint> points, bool horizontally);

    bool intersects(QRect* rect);
    bool isInside(QRect *rect);

    bool isInLeftSubtree(QRect *rect);
public:
    Node(std::vector<QPoint> points);
    ~Node();
    std::vector<QPoint> insideRectangle(QRect* rect);
};

#endif // NODE_H
