#include "Particle.h"
#include "Game.h"

Particle::Particle() //default constructor
	: mass(0.0), radius(0.0), velocity(Point()), position(Point())
{
}

Particle::Particle(Point gPos) 
	: mass(0.0), radius(0.0), velocity(Point()), position(Point(gPos))
{
}

Particle::Particle(Point gPos, double gMass)
	: mass(gMass), radius(mass * radiusProportion), velocity(Point()), position(Point(gPos))
{
	if (radius < minimumSize)
		radius = minimumSize;

	//prevent particles from spawning on the borders
	if (position.getX() <= radius)
		position.setX(radius);
	if (position.getX() >= DEFAULT_WIDTH - radius)
		position.setX(DEFAULT_WIDTH - radius);
	if (position.getY() <= radius) 
		position.setY(radius);
	if (position.getY() >= DEFAULT_HEIGHT - radius)
		position.setY(DEFAULT_HEIGHT - radius);
}

Particle::Particle(Point gPos, Point gVel, double gMass, double gRadius) 
	: mass(gMass), radius(gRadius), velocity(Point(gVel)), position(Point(gPos))
{
}

Particle::~Particle() //destructor
{
	mass = 0.0;
	radius = 0.0;	
}

Point Particle::getPos() const
{
	return position;
}

Point Particle::getVel() const
{
	return velocity;
}

double Particle::getMass() const
{
	return mass;
}

double Particle::getRadius() const
{
	return radius;
}

void Particle::newPosXY(double gX, double gY)
{
	position.setX(gX);
	position.setY(gY);
}

void Particle::newVelXY(double gX, double gY) //set and limit new velocity within velocity limit
{
	if(gX >= upperLimit)
		velocity.setX(upperLimit);
	else if (gX <= lowerLimit)
		velocity.setX(lowerLimit);
	else
		velocity.setX(gX);

	if(gY >= upperLimit)
		velocity.setY(upperLimit);
	else if (gY <= lowerLimit)
		velocity.setY(lowerLimit);
	else
		velocity.setY(gY);
}

void Particle::reverseVelX() //reverse the velocity from '+' to '-', vice versa
{
	velocity.setX(velocity.getX() * -1);
}

void Particle::reverseVelY()
{
	velocity.setY(velocity.getY() * -1);
}

void Particle::calcPos(double dt) //calculate and set new x and y position
{
	position.setX(position.getX() + (velocity.getX() * dt));
	position.setY(position.getY() + (velocity.getY() * dt));
}

Point Particle::calcVelElastic(const Particle& other)
{
	double vX, vY, v1, v2, m1, m2, t1, t2, AOC;

	v1 = sqrt(pow(velocity.getX(), 2) + pow(velocity.getY(), 2)); //magnitudes of velocity1
	v2 = sqrt(pow(other.getVel().getX(), 2) + pow(other.getVel().getY(), 2)); //magnitudes of velocity2
	m1 = mass; //mass1
	m2 = other.getMass(); //mass2
	t1 = angleOfVel(); //theta1
	t2 = other.angleOfVel(); //theta2
	AOC = other.getPos().getAngleRad(position); //Angle of Contact for collision

	vX = (((v1 * cos(t1 - AOC) * (m1 - m2)) + (2 * m2 * v2 * cos(t2 - AOC))) / (m1 + m2)) * cos(AOC) 
		+ (v1 * sin(t1 - AOC) * cos(AOC + PI / 2));

	vY = (((v1 * cos(t1 - AOC) * (m1 - m2)) + (2 * m2 * v2 * cos(t2 - AOC))) / (m1 + m2)) * sin(AOC)
		+ (v1 * sin(t1 - AOC) * sin(AOC + PI / 2));

	Point newVel(vX, vY);
	
	return newVel;
}

double Particle::angleOfVel() const //calculate angles of movement of the particle
{
	double result = atan2(velocity.getY(), velocity.getX());
	return result;
}

bool Particle::collide(const Particle& other) //check for collision between particles
{
	double d = position.getDistance(other.getPos());
	if ((radius + other.getRadius()) >= d)
		return true;
	else
		return false;
}

