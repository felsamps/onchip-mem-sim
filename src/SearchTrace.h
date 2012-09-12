#ifndef _SEARCHTRACE_H
#define	_SEARCHTRACE_H

#include <iostream>
#include <list>
#include <string>

using namespace std;

class SearchTrace {
private:
    list<list<string> > meTrace;
    list<list<string> > deTrace;

public:
    SearchTrace();
    
    void insertMeTrace(list<string> trace);
    void insertDeTrace(list<string> trace);

    list<string> getMeTrace();
    list<string> getDeTrace();

    int getMeListSize();
    int getDeListSize();
 
};

#endif	/* _SEARCHTRACE_H */

