#include "SearchTrace.h"

SearchTrace::SearchTrace() {
	meTrace.clear();
	deTrace.clear();
}

void SearchTrace::insertMeTrace(list<Entry*> trace, pair<int,int> p) {
	list<Entry*> l;
	while(!trace.empty()) {
		l.push_back(trace.front());
		trace.pop_front();
	}
	pair<int,int> curr(p);
	meTrace.push_back(l);
	meCurr.push_back(curr);
}

void SearchTrace::insertDeTrace(list<Entry*> trace, pair<int,int> p) {
	list<Entry*> l;
	while(!trace.empty()) {
		l.push_back(trace.front());
		trace.pop_front();
	}
	pair<int,int> curr(p);
	deTrace.push_back(l);
	deCurr.push_back(curr);
}

list<Entry*> SearchTrace::getMeTrace() {
	list<Entry*> returnable = meTrace.front();
	meTrace.pop_front();
	return returnable;
}

list<Entry*> SearchTrace::getDeTrace() {
	list<Entry*> returnable = deTrace.front();
	deTrace.pop_front();
	return returnable;
}

pair<int,int> SearchTrace::getMeCurr() {
	pair<int,int> returnable = meCurr.front();
	meCurr.pop_front();
	return returnable;
}

pair<int,int> SearchTrace::getDeCurr() {
	pair<int,int> returnable = deCurr.front();
	deCurr.pop_front();
	return returnable;
}

int SearchTrace::getMeListSize() {
	return meTrace.size();
}

int SearchTrace::getDeListSize() {
	return deTrace.size();
}