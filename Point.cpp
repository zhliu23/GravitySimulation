#include "Point.h"

Point::Point() : x(0), y(0)
{
}

Point::Point(double gX, double gY) : x(gX), y(gY)
{
}

Point::Point(const Point& other) : x(other.getX()), y(other.getY())
{
}

Point::~Point()
{
	x = 0.0;
	y = 0.0;
}

double Point::getX() const
{
	return x;
}

double Point::getY() const
{
	return y;
}

void Point::setX(double gX)
{
	x = gX;
}

void Point::setY(double gY)
{
	y = gY;
}

double Point::getDistance(const Point& otherPos)
{
	double x2 = otherPos.getX(), y2 = otherPos.getY();
	double r = sqrt(pow(x2 - x, 2) + pow(y2 - y, 2));
	return r;
}

double Point::getAngleRad(const Point& otherPos)
{
	double x2 = otherPos.getX(), y2 = otherPos.getY();
	double result = atan2(y2 - y, x2 - x);
	return result;
}






