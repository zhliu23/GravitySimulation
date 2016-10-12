#ifndef POINT_H
#define	POINT_H

#include <cmath>

class Point
{
private:
	double x;
	double y;
public:
	//constructors + destructor
	Point();
	Point(double x, double y);
	Point(const Point& other);
	~Point();
	//getters
	double getX() const;
	double getY() const;
	//setters
	void setX(double gX);
	void setY(double gY);
	//helper functions
	double getDistance(const Point& otherPos);
	double getAngleRad(const Point& otherPos);
};

#endif