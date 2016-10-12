#ifndef PARTICLE_H
#define	PARTICLE_H

#include "Point.h"

//limit for velocity
const double upperLimit = 700.0;
const double lowerLimit = -700.0;
//minimum particle size
const double minimumSize = 2.0;
const double radiusProportion = 0.85e-14;
const double PI = 3.14;

class Particle
{
private:
	Point position;
	Point velocity;
	double mass;
	double radius;
public:
	//constructors + destructor
	Particle();
	Particle(Point gPos);
	Particle(Point gPos, double gMass);
	Particle(Point gPos, Point gVel, double gMass, double gRadius);
	~Particle();
	//getters
	Point getPos() const;
	Point getVel() const;
	double getMass() const;
	double getRadius() const;
	//setters
	void newPosXY(double gX, double gY);
	void newVelXY(double gX, double gY);
	//helper functions:
	void reverseVelX();
	void reverseVelY();
	void calcPos(double dt);
	Point calcVelElastic(const Particle& other);
	double angleOfVel() const;
	bool collide(const Particle& other);
};

#endif
