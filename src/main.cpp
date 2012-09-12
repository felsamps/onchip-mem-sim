#include <cstdlib>
#include <string>

#include "VideoHandler.h"


using namespace std;

int main(int argc, char** argv) {

	int nv = atoi(argv[1]);
	int nf = atoi(argv[2]);
	int w = atoi(argv[3])/16;
	int h = atoi(argv[4])/16;
	int sr = atoi(argv[5]);
	string traceFileName(argv[6]);
	string offLineStatsFileName(argv[7]);
	
	VideoHandler vh(nv, nf, w, h);
	
	return (EXIT_SUCCESS);
}

