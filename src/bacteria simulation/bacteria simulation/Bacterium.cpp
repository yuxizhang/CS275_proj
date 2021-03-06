#define _USE_MATH_DEFINES

#include "Bacterium.h"
#include "Helper.h"
#include <cmath>
#include <sstream>
#include <climits>
#include <fstream>

#if !defined(_WIN32)
#include "FANN.h"
#endif

using namespace std;

std::default_random_engine generator;
std::uniform_real_distribution<double> uniform(0.0, 1.0);
std::geometric_distribution<int> geometric(0.6);

double Bacterium::getSpeed() {
	return BASE_SPEED / (radius / BASE_SIZE);
}

string Bacterium::getRecord(vector<Bacterium>& bacteria) {
	stringstream ss;
	ss << radius;
	for (int i = 0; i < bacteria.size(); ++i) {
		ss << " " << bacteria[i].positionX - positionX << " " << bacteria[i].positionY - positionY << " " 
			<< bacteria[i].radius << " " << bacteria[i].energy;
	}
	ss << endl;
	ss << cos(theta) << " " << sin(theta) << endl;
	// ss << theta << endl;
	return ss.str();
}

void Bacterium::move() {

	double speed = getSpeed();
	double directionX = speed * cos(theta);
	double directionY = speed * sin(theta);
	if (positionX + directionX > 1 || positionX + directionX < 0) directionX = -directionX;
	if (positionY + directionY > 1 || positionY + directionY < 0) directionY = -directionY;
	positionX += directionX;
	positionY += directionY;

	energy -= radius * radius * 0.01 * radius / BASE_SIZE;
	++age;
}


void Bacterium::initialize(int id, int smart) {
	/*
		smart level:
		0 : random
		1 : naive ai
		2 : nn smart ai
		3 : 1/3 prob be 0/1/2
	*/
	smartlevel = smart;
	if (smartlevel == 3) smartlevel = rand() % 3;

	netfile = NetFile;
	this->id = id;
	if (smartlevel == 2) this->id = -this->id;
	positionX = uniform(generator);
	positionY = uniform(generator);
	radius = (1 + geometric(generator)) * BASE_SIZE;
	energy = radius * radius;
	theta = (uniform(generator) - 0.5) * 2 * M_PI;
	age = 0;
}

void Bacterium::updateDirection(vector<Bacterium>& bacteria) {
	if (uniform(generator) < 0.7) {
		return;
	}
	theta = (uniform(generator) - 0.5) * 2 * M_PI;
	if (smartlevel == 1) {
		double mindist = INT_MAX;
		for (int i = 0; i < bacteria.size(); i++)
		{
			if ((bacteria[i].radius > radius) && (dist(bacteria[i]) < mindist)) {
				mindist = dist(bacteria[i]);
				double dy = positionY - bacteria[i].positionY;
				double dx = positionX - bacteria[i].positionX;
				theta = atan2(dy, dx);
			}
		}
	} 
	#if !defined(_WIN32)
	else if (smartlevel == 2) {
		ifstream file(netfile);
		if (file) {
			fann_type* input = new fann_type[NEIGHBOUR_SIZE * 4 + 1];
			input[0] = radius;
			for (int i = 0; i < bacteria.size(); i++)
			{
				input[4 * i + 1] = bacteria[i].positionX - positionX;
				input[4 * i + 2] = bacteria[i].positionY - positionY;
				input[4 * i + 3] = bacteria[i].radius;
				input[4 * i + 4] = bacteria[i].energy;
			}
			double dirX,dirY;
			FANN_Test(netfile, input, dirX, dirY);
			
			theta = atan2(dirX, dirY);
			//cout << "dirX, dirY: " << dirX << " " << dirY << " " << theta << endl;

			delete[] input;
		}
	}
	#endif
}

bool Bacterium::touch(const Bacterium& b) {
	return (radius + b.radius) * (radius + b.radius) >
		(positionX - b.positionX) * (positionX - b.positionX) +
		(positionY - b.positionY) * (positionY - b.positionY);
}

double Bacterium::dist(const Bacterium& b) {
	return sqrt(
		(positionX - b.positionX) * (positionX - b.positionX) + 
		(positionY - b.positionY) * (positionY - b.positionY));
}

