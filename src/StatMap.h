#ifndef _STATMAP_H
#define	_STATMAP_H

#include <iostream>
#include <cmath>
#include <fstream>

#include "Defines.h"


using namespace std;

class StatMap {
private:
    double mean, std;
    long long int **map;
    bool **tempMap;
    int dim, searchRange;

    void xInitMap(int sr);

public:
    
    StatMap(int sr);
    StatMap(int sr, string fileName);
    StatMap(int sr, int dMe, StatMap* mapMe, int dDe, StatMap* mapDe);

    void insertAccess(pair<int,int> p);
    long long int getStateMapPos(int x, int y);
    void setStateMapPos(int x, int y, long long int v);
    void updateStateMap();
    void generateStats();

    PowerState getPowerState(int x, int y);
    pair<double, double> getStats();

};

#endif	/* _STATMAP_H */

