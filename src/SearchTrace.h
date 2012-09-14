#ifndef _SEARCHTRACE_H
#define	_SEARCHTRACE_H

#include <iostream>
#include <list>
#include <string>

#include "Entry.h"

using namespace std;

class SearchTrace {
private:
    list<list<Entry*> > meTrace;
    list<list<Entry*> > deTrace;

    list<pair<int,int> > meCurr;
    list<pair<int,int> > deCurr;


public:
    SearchTrace();
    
    void insertMeTrace(list<Entry*> trace, pair<int,int> p);
    void insertDeTrace(list<Entry*> trace, pair<int,int> p);

    list<Entry*> getMeTrace();
    list<Entry*> getDeTrace();

    pair<int,int> getMeCurr();
    pair<int,int> getDeCurr();

    int getMeListSize();
    int getDeListSize();
 
};

#endif	/* _SEARCHTRACE_H */

