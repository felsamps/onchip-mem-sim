#include <iostream>
#include <cstdlib>
#include <string>

#include "VideoHandler.h"
#include "OnchipMemory.h"
#include "SearchEngine.h"


using namespace std;

int main(int argc, char** argv) {

	int nv = atoi(argv[1]);
	int ngop = atoi(argv[2]);
	int w = atoi(argv[3])/16;
	int h = atoi(argv[4])/16;
	int sr = atoi(argv[5]);
	string traceFileName(argv[6]);
	string offlineStatsMe(argv[7]);
	string offlineStatsDe(argv[8]);

	int nf = ngop * GOP_SIZE + 1;
	
	VideoHandler* vh = new VideoHandler(nv, nf, w, h, traceFileName);
	cout << "Video Handler Creation (Complete)" << endl;
	vh->parseTraceFile();
	cout << "Trace File Parsing (Complete)" << endl;
	OnchipMemory* mem = new OnchipMemory(sr, nv, nf, w, h);
	cout << "OnChip Memory Definition (Complete)" << endl;
	SearchEngine se(nv, ngop, w, h, vh, mem, offlineStatsMe, offlineStatsDe);
	cout << "Search Engine Creation (Complete)" << endl;
	se.performSearch();

	se.report();
	
	return (EXIT_SUCCESS);
}

