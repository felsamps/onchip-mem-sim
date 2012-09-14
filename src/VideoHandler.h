#ifndef _VIDEOHANDLER_H
#define	_VIDEOHANDLER_H

#include <iostream>
#include <fstream>

#include "SearchTrace.h"

class VideoHandler {
private:
    SearchTrace *****video;
    fstream traceFile;
public:
    VideoHandler(int nv, int nf, int w, int h, string traceFileName);
    void parseTraceFile();

    SearchTrace* getSearchTrace(int view, int frame, int x, int y);
};

#endif	/* _VIDEOHANDLER_H */

