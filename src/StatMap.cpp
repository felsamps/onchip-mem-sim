#include "StatMap.h"

StatMap::StatMap(int sr) {
	xInitMap(sr);

	this->mean = 0.0;
	this->std = 0.0;

	for (int x = 0; x < this->dim; x++) {
		for (int y = 0; y < this->dim; y++) {
			this->map[x][y] = 0;
			this->tempMap[x][y] = false;
		}
	}
}

StatMap::StatMap(int sr, string fileName) {
	fstream file(fileName.c_str(), fstream::in);
	
	xInitMap(sr);

	file >> this->mean >> this->std;

	for (int y = 0; y < this->dim; y++) {
		for (int x = 0; x < this->dim; x++) {
			file >> this->map[y][x];
		}
	}
}

StatMap::StatMap(int sr, int dMe, StatMap* mapMe, int dDe, StatMap* mapDe) {
	xInitMap(sr);

	for (int y = 0; y < this->dim; y++) {
		for (int x = 0; x < this->dim; x++) {
			long long int n = (dMe * mapMe->getStateMapPos(x, y)) + (dDe * mapDe->getStateMapPos(x, y));
			this->map[x][y] = n;
		}
	}
	
}

void StatMap::xInitMap(int sr) {
	this->dim = (sr/BLOCK_SIZE)*2 + 1;
	this->searchRange = sr;

	this->map = new long long int*[this->dim];
	this->tempMap = new bool*[this->dim];
	
	for (int i = 0; i < this->dim; i++) {
		this->map[i] = new long long int[this->dim];
		this->tempMap[i] = new bool[this->dim];
	}
}

void StatMap::insertAccess(pair<int,int> p) {
	/*TODO need a revision */
	this->tempMap[p.first][p.second] = true;
}


void StatMap::updateStateMap() {
	for (int y = 0; y < this->dim; y++) {
		for (int x = 0; x < this->dim; x++) {
			this->map[x][y] += (this->tempMap[x][y]) ? 1 : 0;
			this->tempMap[x][y] = false;
		}
	}
}

void StatMap::generateStats() {
	long long int acum = 0;
	for (int y = 0; y < this->dim; y++) {
		for (int x = 0; x < this->dim; x++) {
			acum += this->map[x][y];
		}
	}
	this->mean = acum/pow(this->dim, 2);

	double dAcum;
	for (int y = 0; y < this->dim; y++) {
		for (int x = 0; x < this->dim; x++) {
			dAcum += pow(this->map[x][y] - mean, 2);
		}
	}
	this->std = pow((dAcum / (pow(this->dim,2) -1)), 0.5);
}

PowerState StatMap::getPowerState(int x, int y) {
	/* TODO improve weights */
	double th0 = this->mean - 0.5 * this->std;
	double th1 = this->mean + 1 * this->std;

	long long int pos = this->map[x][y];
	if(this->mean != 0) {
		return (pos < th0)				? S1 :
			   (th0 < pos && pos < th1) ? S2 :
										  S3 ;
										
	}
	else {
		return S0;
	}
}

void StatMap::setStateMapPos(int x, int y, long long int v) {
	this->map[x][y] = v;
}

long long int StatMap::getStateMapPos(int x, int y) {
	return this->map[x][y];
}

pair<double,double> StatMap::getStats() {
	pair<double,double> p(this->mean, this->std);
	return p;
}