#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <random>
#include <cmath>
#include <time.h>
using namespace std;

#include "Bacterium.h"
#include "Environment.h"

#if !defined(_WIN32)
#include "FANN.h"
#endif

#include "Helper.h"

int main() {
	srand (time(NULL));
	int niters = 1;
	Environment base_environment(20, 0, 1);
	// base_environment.display();
	base_environment.run(500);
	base_environment.CloseFile();
// 	for (int iter = 0; iter < niters; iter++) {
// #if !defined(_WIN32)
// 		FANN_Train(DataFile, NetFile);
// #endif
// 		printf("train completed\n");
// 		Environment environment(20, 1, 0.5);
// 		// environment.display();
// 		environment.run(500);
// 	}
	printf("Completed!\n");
	// system("pause");
}