#ifndef _ONCHIPMEMORY_H
#define	_ONCHIPMEMORY_H

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <iostream>

#include "Defines.h"
#include "StatMap.h"

using namespace std;

class OnchipMemory {
private:
    /* Parameters */
    int searchRange, dim;
    int nv, nf, w, h;
    int nMe, nDe;

    /* PowerMap */
    PowerState **powerMap;
    vector<set<pair<int,int> > > stateSet;

    /* State Maps */
    StatMap *currMeMap, *currDeMap, *currMap;
    StatMap *nextMeMap, *nextDeMap;

    /* Energy Accumulators */
    long long int s0, s1, s2, s3; /* leakage energy accumulators */
    long long int w03, w13, w23;  /* wake-up energy accumulators */
    long long int timeInstantCounter;

    long long int mergedAcc, nonMergedAcc;
    set<pair<int,int> > mergedCandidates, nonMergedCandidates;

public:
    OnchipMemory(int sr, int nv, int nf, int w, int h);

    void initFrame(int dMe, int dDe, string fileNameMe, string fileNameDe);
    void initFrame(int dMe, int dDe);

    void insertAccess(int currView, int refView, int posx, int posy);
    void updateNextStatMap();
    void resetPowerMap();
    
    StatMap* xMergeStatMaps(int dMe, StatMap* mapMe, int dDe, StatMap* mapDe);
    void xUpdatePowerMap();
    pair<int,int> xCalcMemPosition(int x, int y);

    void reportPowerMap();
    void report();

    void mergeCandidate(pair<int,int> c);
    void resetMergedCandidates();
    void resetNonMergedCandidates();
    void countNumOfSearches(int dMe,int dDe);

};

#endif	/* _ONCHIPMEMORY_H */

