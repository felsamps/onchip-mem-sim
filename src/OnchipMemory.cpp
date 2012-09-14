#include "OnchipMemory.h"

OnchipMemory::OnchipMemory(int sr, int nv, int nf, int w, int h) {
	this->searchRange = sr;
	this->dim = (sr/BLOCK_SIZE)*2 + 1;
	this->nv = nv;
	this->nf = nf;
	this->w = w;
	this->h = h;

	this->nMe = 0;
	this->nDe = 0;
	
	this->powerMap = new PowerState*[this->dim];
	for (int y = 0; y < this->dim; y++) {
		this->powerMap[y] = new PowerState[this->dim];
	}

	stateSet.resize(4);

	s0 = 0;
	s1 = 0;
	s2 = 0;
	s3 = 0;
	w03 = 0;
	w13 = 0;
	w23 = 0;
	timeInstantCounter = 0;

	mergedAcc = 0;
	nonMergedAcc = 0;
	mergedCandidates.clear();
	nonMergedCandidates.clear();
	
}

void OnchipMemory::initFrame(int dMe, int dDe) {

	this->currMap = xMergeStatMaps(dMe, this->nextMeMap, dDe, this->nextDeMap);
	
	this->currMeMap = nextMeMap;
	this->currDeMap = nextDeMap;

	this->nextMeMap = new StatMap(this->searchRange);
	this->nextDeMap = new StatMap(this->searchRange);

	xUpdatePowerMap();
	
}

void OnchipMemory::initFrame(int dMe, int dDe, string fileNameMe, string fileNameDe) {
	this->currMeMap = new StatMap(this->searchRange, fileNameMe);
	this->currDeMap = new StatMap(this->searchRange, fileNameDe);

	this->nextMeMap = new StatMap(this->searchRange);
	this->nextDeMap = new StatMap(this->searchRange);

	this->currMap = xMergeStatMaps(dMe, this->currMeMap, dDe, this->currDeMap);
	xUpdatePowerMap();
}

StatMap* OnchipMemory::xMergeStatMaps(int dMe, StatMap* mapMe, int dDe, StatMap* mapDe) {
	StatMap* merged = new StatMap(this->searchRange, dMe, mapMe, dDe, mapDe);
	merged->generateStats();
	pair<double,double> stats = merged->getStats();
	if(stats.first == 0) {
		return this->currMap;
	}
	else {
		return merged;
	}
}

void OnchipMemory::xUpdatePowerMap() {
	for (int i = 0; i < 4; i++) {
		this->stateSet[i].clear();
	}

	for (int y = 0; y < this->dim; y++) {
		for (int x = 0; x < this->dim; x++) {
			pair<int,int> p(x,y);
			this->powerMap[x][y] = this->currMap->getPowerState(x,y);
			this->stateSet[this->powerMap[x][y]].insert(p);
		}
	}
}

pair<int,int> OnchipMemory::xCalcMemPosition(int x, int y) {
	//TODO
	int offset = this->searchRange/BLOCK_SIZE;
	int retX = offset + (((x<0) ? x-1 : x) / BLOCK_SIZE);
	int retY = offset + (((y<0) ? y-1 : y) / BLOCK_SIZE);

	/*normalize to the search window limits */
	retX = (retX > this->dim-1) ? this->dim-1 : retX;
	retX = (retX < 0) ? 0 : retX;
	retY = (retY > this->dim-1) ? this->dim-1 : retY;
	retY = (retY < 0) ? 0 : retY;

	pair<int,int> p(retX, retY);
	return p;	
}

void OnchipMemory::insertAccess(int currView, int refView, int x, int y) {
	/* here everything happens... */
	/* TODO calculate the four accessed blocks in the onchip memory */
		
	vector<pair<int,int> > acc;
	acc.push_back(xCalcMemPosition(x, y));
	acc.push_back(xCalcMemPosition(x + BLOCK_SIZE -1, y));
	acc.push_back(xCalcMemPosition(x, y + BLOCK_SIZE -1));
	acc.push_back(xCalcMemPosition(x + BLOCK_SIZE -1, y + BLOCK_SIZE -1));
		
	/* TODO update the counters */
	/* TODO update the powermap */
	/* TODO update the next statmaps */
	for (int i = 0; i < 4; i++) {
		int bx = acc[i].first;
		int by = acc[i].second;
		
		switch(this->powerMap[bx][by]) {
			case S0:
				this->w03 += 1;
				stateSet[S0].erase(acc[i]);
				stateSet[S3].insert(acc[i]);
				reportPowerMap();
				getchar();
				break;
			case S1:
				this->w13 += 1;
				stateSet[S1].erase(acc[i]);
				stateSet[S3].insert(acc[i]);
				break;
			case S2:
				this->w23 += 1;
				stateSet[S2].erase(acc[i]);
				stateSet[S3].insert(acc[i]);
				break;
			case S3:
				/* do nothing */
				break;
		}

		this->powerMap[bx][by] = S3;

		/* leakage energy accumulation */
		this->s0 += this->stateSet[S0].size();
		this->s1 += this->stateSet[S1].size();
		this->s2 += this->stateSet[S2].size();
		this->s3 += this->stateSet[S3].size();

		this->timeInstantCounter += 1;
		
		if(currView == refView) { /* Motion Estimation */
			this->nextMeMap->insertAccess(acc[i]);
		}
		else { /* Disparity Estimation */
			this->nextDeMap->insertAccess(acc[i]);
		}

		/* Candidates Merging */
		this->mergeCandidate(acc[0]);
	}
}

void OnchipMemory::updateNextStatMap() {
	this->nextDeMap->updateStateMap();
	this->nextMeMap->updateStateMap();

}


void OnchipMemory::resetPowerMap() {
	for (int i = 0; i < 4; i++) {
		this->stateSet[i].clear();
	}

	for (int y = 0; y < this->dim; y++) {
		for (int x = 0; x < this->dim; x++) {
			pair<int,int> p(x,y);
			this->powerMap[x][y] = this->currMap->getPowerState(x,y);
			this->stateSet[this->powerMap[x][y]].insert(p);
		}
	}
}

void OnchipMemory::mergeCandidate(pair<int,int> c) {
	this->nonMergedCandidates.insert(c);
	this->mergedCandidates.insert(c);
}

void OnchipMemory::resetMergedCandidates() {
	this->mergedAcc += mergedCandidates.size();
	this->mergedCandidates.clear();
}

void OnchipMemory::resetNonMergedCandidates() {
	this->nonMergedAcc += nonMergedCandidates.size();
	this->nonMergedCandidates.clear();
}

void OnchipMemory::report() {
	double levelCEnergy = (this->timeInstantCounter * 4 * this->dim * this->dim);
	double levelCPlusEnergy = (this->timeInstantCounter * 4 * this->dim * (this->dim+1));
	double noPGEnergy = (this->timeInstantCounter * this->dim * this->dim);
	double pgEnergy = (this->s3 + this->s2*0.5 + this->s1*0.3 + this->s0*0 + this->w03*0.3 + this->w13*0.2 + this->w23*0.1);
	double savingLevelC = levelCEnergy/levelCPlusEnergy;
	double savingNoPG = noPGEnergy/levelCPlusEnergy;
	double savingPG = pgEnergy/levelCPlusEnergy;
	double savingMerge = (double)this->mergedAcc/(double)this->nonMergedAcc;

	cout << "Energy Report" << endl;
	cout << "#Time:\t" << this->timeInstantCounter << endl;
	cout << "Static Energy: " << endl;
	/* TODO calculate LevelC energy */
	cout << "LevelC+:\t1.0" << endl;
	cout << "LevelC:\t\t" <<  savingLevelC << endl;
	cout << "Ours(noPG):\t" << savingNoPG << endl;
	cout << "Ours:\t\t" << savingPG << endl;
	cout << "Dynamic Energy: " << endl;
	cout << "W/O Merging:\t1.0" << endl;
	cout << "Candidate Merging:\t" << savingMerge << endl;

}

void OnchipMemory::reportPowerMap() {
	cout << "PowerMap Report" << endl;
	for (int y = 0; y < this->dim; y++) {
		for (int x = 0; x < this->dim; x++) {
			cout << this->powerMap[x][y] << " ";
		}
		cout << endl;
	}

}

void OnchipMemory::countNumOfSearches(int dMe, int dDe) {
	this->nMe += dMe;
	this->nDe += dDe;
}