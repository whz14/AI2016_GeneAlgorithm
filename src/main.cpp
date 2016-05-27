// encoding GB2312
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <climits>
#include <fstream>
#include <algorithm>
#include <vector>
#include "track.h"

const double e = 2.71828182846;
const double eps = 0.00001;
const int POPU_NUM = 500;
const int GENERATION_NUM = 2000;
const double Pm = 0.6;
using namespace std;

track population[POPU_NUM];		// the population
track n_population[POPU_NUM];	// new population. to be selected into [track population];

void GA() {	// genetic algorithm
	for(int i = 0; i < POPU_NUM; ++i) {
		population[i].init();
	}
	for(int i = 0; i < GENERATION_NUM; ++i) {
		//cout << "for the " << i << "th generation" << endl;
		clc_p(population, POPU_NUM);
		for(int j = 0; j < POPU_NUM; ++j) {
			n_population[j] = evolution(population, POPU_NUM);	// choose POPU_NUM individuals by their possibility
		}
		random_shuffle(n_population, n_population + POPU_NUM);
		for(int j = 0; 2 * j < POPU_NUM; ++j) {
			mating(n_population[j], n_population[j + POPU_NUM / 2], population[j], population[j + POPU_NUM / 2]);
		}
		for(int j = 0; j < POPU_NUM; ++j) {
			population[j].mutate(Pm);
		}
	}
}

int main(int argc, char** argv) {
	srand(time(NULL));
	if (argc != 3) {
		cout << "error input\n" << "example is MAIN.exe InputFileName OutputFileName\n";
		//system("pause");
		return -1;
	}

	track::init(argv[1]);
	GA();
	ofstream fout(argv[2]);
	track::print(fout);
	fout.close();
	return 0;
}