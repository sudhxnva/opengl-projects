#include "CSCIx229.h"

Point2D cubicBezier(Point2D p0, Point2D p1, Point2D p2, Point2D p3, double t)
{
    double u = 1.0 - t;
    double b0 = u * u * u;
    double b1 = 3 * u * u * t;
    double b2 = 3 * u * t * t;
    double b3 = t * t * t;
    Point2D p;
    p.x = b0 * p0.x + b1 * p1.x + b2 * p2.x + b3 * p3.x;
    p.y = b0 * p0.y + b1 * p1.y + b2 * p2.y + b3 * p3.y;
    return p;
}

Point2D cubicBezierDerivative(Point2D p0, Point2D p1, Point2D p2, Point2D p3, double t)
{
    double u = 1.0 - t;
    Point2D d;
    d.x = 3 * u * u * (p1.x - p0.x) +
          6 * u * t * (p2.x - p1.x) +
          3 * t * t * (p3.x - p2.x);
    d.y = 3 * u * u * (p1.y - p0.y) +
          6 * u * t * (p2.y - p1.y) +
          3 * t * t * (p3.y - p2.y);
    return d;
}