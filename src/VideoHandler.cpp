#include "VideoHandler.h"


VideoHandler::VideoHandler(int nv, int nf, int w, int h) {
	video = new SearchTrace***[nv];
	for (int v = 0; v < nv; v++) {
		video[v] = new SearchTrace**[nf];
		for (int f = 0; f < nf; f++) {
			video[v][f] = new SearchTrace*[w];
			for (int x = 0; x < w; x++) {
				video[v][f][w] = new SearchTrace();
			}
		}
	}
}
