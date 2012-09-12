#ifndef _ENTRY_H
#define	_ENTRY_H

struct Entry {

    char type;
    int nv, pv, nh, ph;
    int x, y;

    Entry(char t, int p0, int p1, int p2, int p3) {
        this->type = t;
        this->nv = p0;
        this->pv = p1;
        this->nh = p2;
        this->ph = p3;
    }

    Entry(char t) {
        this->type = t;
    }

    Entry(char t, int i, int j) {
        this->type = t;
        this->x = i;
        this->y = j;
    }

};

#endif	/* _ENTRY_H */

