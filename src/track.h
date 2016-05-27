#pragma once
#ifndef TRACK_H
#define TRACK_H
#include <iostream>
#include <cmath>
#include <string>

const int CITY_MAX = 100;

class track {
private:
	static double MAX;	// max total distance.
	static int cityNum;
	static std::string cityNames[CITY_MAX];
	static std::pair<double, double> cities[CITY_MAX];
	//static std::string ans;
	static double tmpBst;	// the best (largest MAX-f(), i.e., smallest f()) UP TO present
	static int bstTrace[CITY_MAX];	// the best trace up to present

	int trace[CITY_MAX];
	
	double distance(int i, int j) const {
		return sqrt(pow(cities[trace[i]].first - cities[trace[j]].first, 2) +
			pow(cities[trace[i]].second - cities[trace[j]].second, 2));
	}

	double p;		// possiblity to be chosen. determined together with others
public:
	track();
	track(int trace1[]);
	void init();
	static void init(char*);
	static void print(std::ostream& out = std::cout);
	int citynum() const { return cityNum; }
	
	double f() const;
	void mutate(double m_p); 

	friend void mating(const track& p1, const track& p2, track& c1, track& c2);
	friend void clc_p(track* popu, int num);
	friend track& evolution(track*, int);
};


#endif // !TRACK_H
