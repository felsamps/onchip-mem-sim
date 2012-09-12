#include "SearchTrace.h"

SearchTrace::SearchTrace() {
	meTrace.clear();
	deTrace.clear();
}

void SearchTrace::insertMeTrace(list<string> trace) {
	list<string> l;
	while(!trace.empty()) {
		l.push_back(trace.front());
		trace.pop_front();
	}
	meTrace.push_back(l);
}

void SearchTrace::insertDeTrace(list<string> trace) {
	list<string> l;
	while(!trace.empty()) {
		l.push_back(trace.front());
		trace.pop_front();
	}
	deTrace.push_back(l);
}

list<string> SearchTrace::getMeTrace() {
	list<string> returnable = meTrace.front();
	meTrace.pop_front();
	return returnable;
}

list<string> SearchTrace::getDeTrace() {
	list<string> returnable = deTrace.front();
	deTrace.pop_front();
	return returnable;
}

int SearchTrace::getMeListSize() {
	return meTrace.size();
}

int SearchTrace::getDeListSize() {
	return deTrace.size();
}