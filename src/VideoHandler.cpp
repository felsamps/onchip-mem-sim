#include "VideoHandler.h"
#include "StatMap.h"


VideoHandler::VideoHandler(int nv, int nf, int w, int h, string traceFileName) {
	this->video = new SearchTrace****[nv];
	for (int v = 0; v < nv; v++) {
		this->video[v] = new SearchTrace***[nf];
		for (int f = 0; f < nf; f++) {
			this->video[v][f] = new SearchTrace**[w];
			for (int x = 0; x < w; x++) {
				this->video[v][f][x] = new SearchTrace*[h];
				for (int y = 0; y < h; y++) {
					this->video[v][f][x][y] = new SearchTrace();
				}

			}
		}
	}

	this->traceFile.open(traceFileName.c_str(), fstream::in);
}

void VideoHandler::parseTraceFile() {
	int x, y, cv, cf, rv, rf;
	list<Entry*> l;
	Entry *e;

	cout << "Parsing trace file..." << endl;

	while(!this->traceFile.eof()) {
		char type;
		this->traceFile >> type;
		switch(type) {
			case 'b':
				this->traceFile >> x >> y >> cv >> cf >> rv >> rf;
				break;

			case 'f':
				e = new Entry(type);
				l.push_back(e);
				break;

			case 'r':
				int nv, pv, nh, ph;
				this->traceFile >> nv >> pv >> nh >> ph;
				e = new Entry(type, nv, pv, nh, ph);
				l.push_back(e);
				break;

			case 'c':
				int i, j;
				this->traceFile >> i >> j;
				e = new Entry(type, i, j);
				l.push_back(e);
				break;

			case 'e':
				pair<int,int> p(cv, cf);
				if(cv == rv) { /* Motion Estimation */
					this->video[rv][rf][x][y]->insertMeTrace(l, p);
				}
				else { /* Disparity Estimation */
					this->video[rv][rf][x][y]->insertDeTrace(l, p);
				}
				l.clear();
				break;
		}
		
	}

}

SearchTrace* VideoHandler::getSearchTrace(int view, int frame, int x, int y) {
	return this->video[view][frame][x][y];
}