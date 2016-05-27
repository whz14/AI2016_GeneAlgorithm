#include "track.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <cassert>

using namespace std;

const double M = 50;	// 线性加速适应函数
const double eps = 1e-2;
const double Pc = 0.9;

int track::cityNum = 0;
string track::cityNames[] = {""};
pair<double, double> track::cities[CITY_MAX] = {};
double track::tmpBst = 0;
int track::bstTrace[] = {0};
double track::MAX = 10000;	// changed

track::track() {
	memset(trace, 0, sizeof(trace));
}

track::track(int trace1[]) {
	copy(trace1, trace1 + cityNum, trace);
}

void track::init(char* fileName) {// read city names and positions.
	ifstream in(fileName);
	if(!in.is_open()) {
		cout << "open failed\n";
		cout << fileName << endl;
		// system("dir");
		system("pause");
	}
	in >> cityNum;
	for(int i = 0; i < cityNum; ++i) {
		in >> cityNames[i] >> cities[i].first >> cities[i].second;
		bstTrace[i] = i;
	}
	random_shuffle(bstTrace, bstTrace + cityNum);
}

void track::init() {
	copy(bstTrace, bstTrace + cityNum, trace);
	random_shuffle(trace, trace + cityNum);
}

double track::f() const {	// MAX - ttldistance
	double ans = 0;
	for(int i = 1; i < cityNum; ++i) {
		ans += distance(i, i-1);
	}
	ans += distance(0, cityNum - 1);
	return 100 / ans;
}

void track::mutate(double m_p) {	// mutate(reverse) with the possibiliry of m_p
	if(rand() > RAND_MAX * m_p)		// a possiblity of m_p to mutate
		return;
	int x = rand() % cityNum, y = rand() % cityNum;

	//reverse
	if(x > y) {
		swap(x, y);
	}
	for(; x < y; ++x) {
		swap(trace[x], trace[y]);
	}
}

void track::print(ostream & out) {
	for(int i = 0; i < cityNum; ++i) {
		out << cityNames[bstTrace[i]] << ' ';
	}
	out << 100 / tmpBst << endl;
}

void clc_p(track* popu, int num) {	// 线性加速适应函数
	double sum = 0;
	double& max = track::tmpBst;
	double max1 = 0;
	for(int i = 0; i < num; ++i) {
		sum += popu[i].f();
		if(popu[i].f() > max) {
			max1 = popu[i].f();
			max = popu[i].f();
			copy(popu[i].trace, popu[i].trace + track::cityNum, track::bstTrace);
		}
		else if(popu[i].f() > max1) {
			max1 = popu[i].f();
		}
	}
	double alpha = sum * (M - 1) / (num*max1 - sum);
	double beta = sum*(M*sum - max1*num) / num / (sum - max1*num);
	for(int i = 0; i < num; ++i) {
		popu[i].p = (alpha * popu[i].f() + beta) / sum;
	}
}

track& evolution(track* n_p, int num) {	// 轮盘赌
	double r = rand()*1.0 / RAND_MAX;
	double s = 0;
	for(int i = 0; i < num; ++i) {
		//assert(n_p[i].p <= 1+eps && n_p[i].p >= -1*eps);
		if(s <= r && r < s + n_p[i].p) {
			return n_p[i];
		}
		s += n_p[i].p;
	}
	return n_p[num - 1];
}

void mating(const track& p1, const track& p2, track& c1, track& c2) {	// 常规交叉法
	if(rand() > RAND_MAX * Pc) {
		c1 = p1;
		c2 = p2;
		return;
	}
	int posi = rand() % track::cityNum;
	bool flags1[CITY_MAX] = {false}, flags2[CITY_MAX] = {false};
	int c01[CITY_MAX] = {0}, c02[CITY_MAX] = {0};
	for(int i = 0; i < posi; ++i) {
		flags1[p1.trace[i]] = 1;
		c01[i] = p1.trace[i];
		flags2[p2.trace[i]] = 1;
		c02[i] = p2.trace[i];
	}
	int next1 = posi, next2 = posi;
	for(int i = 0; i < track::cityNum; ++i) {
		if(!flags1[i]) {
			flags1[i] = 1;
			c01[next2++] = i;
		}
		if(!flags2[i]) {
			flags2[i] = 1;
			c02[next1++] = i;
		}
	}
	c1 = track(c02);
	// parents compete with children as well.
	if(c1.f() < p1.f()) {
		c1 = p1;
	}
	c2 = track(c01);
	if(c2.f() < p2.f()) {
		c2 = p2;
	}
}
