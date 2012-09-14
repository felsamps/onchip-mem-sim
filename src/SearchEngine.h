#ifndef _SEARCHENGINE_H
#define	_SEARCHENGINE_H

#include "VideoHandler.h"
#include "OnchipMemory.h"


using namespace std;

class SearchEngine {
private:
    int nv, ngop, w, h;
    VideoHandler* vh;
    OnchipMemory* memory;
    string offMe, offDe;

    void xSearchTraceHandle(int view, SearchTrace* st);
    void xInsertRasterSearch(int currView, int refView, Entry* e);
    void xInsertFirstSearch(int currView, int refView);

public:
    SearchEngine(int nv, int ngop, int w, int h, VideoHandler* vh, OnchipMemory* memory, string offMe, string offDe);


    void performSearch();
    void report();
};

#endif	/* _SEARCHENGINE_H */

