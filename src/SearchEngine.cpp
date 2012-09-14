#include "SearchEngine.h"

int GOP_8_ORDER[9] = {0,8,4,2,6,1,3,5,7};
int VIEW_ORDER[8] = {0,2,1,4,3,6,5,7};

/* TODO */
int TZ_FIRST_SEARCH[65][2] = { {0,-1}, {-1,0}, {1,0}, {0,1}, {0,-2}, {-1,-1}, {1,-1}, {-2,0}, {2,0}, {-1,1}, {1,1}, {0,2}, {0,-4}, {-2,-2}, {2,-2}, {-4,0}, {4,0}, {-2,2}, {2,2}, {0,4}, {0,-8}, {-4,-4}, {4,-4}, {-8,0}, {8,0}, {-4,4}, {4,4}, {0,8}, {0,-16}, {-16,0}, {16,0}, {0,16}, {-4,-12}, {4,-12}, {-4,12}, {4,12}, {-8,-8}, {8,-8}, {-8,8}, {8,8}, {-12,-4}, {12,-4}, {-12,4}, {12,4}, {32,0}, {0,32}, {-8,-24}, {8,-24}, {-8,24}, {8,24}, {-16,-16}, {16,-16}, {-16,16}, {16,16}, {-24,-8}, {24,-8}, {-24,8}, {24,8}, {64,0}, {0,64}, {-16,48}, {16,48}, {32,32}, {48,-16}, {48,16} };
int NUM_OF_FIRST_SEARCH_CANDIDATES = 65;

SearchEngine::SearchEngine(int nv, int ngop, int w, int h, VideoHandler* vh, OnchipMemory* memory, string offMe, string offDe) {
	this->nv = nv;
	this->ngop = ngop;
	this->w = w;
	this->h = h;

	this->vh = vh;
	this->memory = memory;
	this->offMe = offMe;
	this->offDe = offDe;
}

void SearchEngine::performSearch() {
	for (int v = 0; v < nv; v++) {
		
		for (int gop = 0; gop < ngop; gop++) {
			for (int f = (gop == 0) ? 0 : 1; f < GOP_SIZE+1; f++) {
				int view = VIEW_ORDER[v];
				int poc = gop*GOP_SIZE + GOP_8_ORDER[f];
				
				/* frame initialization */
				int dMe = this->vh->getSearchTrace(view, poc, 0, 0)->getMeListSize();
				int dDe = this->vh->getSearchTrace(view, poc, 0, 0)->getDeListSize();

				memory->countNumOfSearches(dMe, dDe);

				cout << "View: " << view << " Frame: " << poc << " DME: " << dMe << " DDE: " << dDe << endl;
				
				if(view==0 && gop==0 && f==0) { // Off-line statistics for the first frame
					memory->initFrame(dMe, dDe, this->offMe, this->offDe);
				}
				else {
					memory->initFrame(dMe,dDe);
				}
				for (int x = 0; x < w; x++) {
					for (int y = 0; y < h; y++) {
						xSearchTraceHandle(view, this->vh->getSearchTrace(view, poc, x, y));
						memory->resetPowerMap();
						memory->resetMergedCandidates();
					}
				}
				
				
			}
		}
	}
}

void SearchEngine::xSearchTraceHandle(int view, SearchTrace* st) {

	while(st->getMeListSize() != 0) {
		list<Entry*> trace = st->getMeTrace();
		pair<int,int> curr = st->getMeCurr();
		
		for(list<Entry*>::iterator it = trace.begin(); it != trace.end(); it++) {
			Entry* e = (*it);
			switch(e->type) {
				case 'f': /* TZ first search */
					xInsertFirstSearch(curr.first, view);
					break;
				case 'r': /* TZ raster search */
					//xInsertRasterSearch(curr.first, view, e);
					break;
				case 'c': /* TZ cadidate block */
					memory->insertAccess(curr.first, view, e->x, e->y);
					break;
			}
		}
		memory->resetNonMergedCandidates();
	}

	while(st->getDeListSize() != 0) {
		list<Entry*> trace = st->getDeTrace();
		pair<int,int> curr = st->getDeCurr();

		for(list<Entry*>::iterator it = trace.begin(); it != trace.end(); it++) {
			Entry* e = (*it);
			switch(e->type) {
				case 'f': /* TZ first search */
					xInsertFirstSearch(curr.first, view);
					break;
				case 'r': /* TZ raster search */
					//xInsertRasterSearch(curr.first, view, e);
					break;
				case 'c': /* TZ cadidate block */
					memory->insertAccess(curr.first, view, e->x, e->y);
					break;
			}
		}
		memory->resetNonMergedCandidates();
	}
	memory->updateNextStatMap();
	
}

void SearchEngine::xInsertRasterSearch(int currView, int refView, Entry* e) {
	for (int y = e->nv; y < e->pv; y+=16) {
		for (int x = e->nh; x < e->ph; x+=16) {
			this->memory->insertAccess(currView, refView, x, y);
		}
	}

}

void SearchEngine::xInsertFirstSearch(int currView, int refView) {
	for (int c = 0; c < NUM_OF_FIRST_SEARCH_CANDIDATES; c++) {
		this->memory->insertAccess(currView, refView, TZ_FIRST_SEARCH[c][0], TZ_FIRST_SEARCH[c][1]);
	}
}

void SearchEngine::report() {
	this->memory->report();
}