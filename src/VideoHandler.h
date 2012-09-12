#ifndef _VIDEOHANDLER_H
#define	_VIDEOHANDLER_H

#include <fstream>

#include "SearchTrace.h"

class VideoHandler {
private:
    SearchTrace *****video;
    fstream traceFile;
public:
    VideoHandler(int nv, int nf, int w, int h, string traceFileName);
    void parseTraceFile();

    
};

#endif	/* _VIDEOHANDLER_H */

