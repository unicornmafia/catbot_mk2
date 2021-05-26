#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "pattern.h"

class Square : public Pattern {

    void getNextCorner(int &newDutyX, int &newDutyY);

    public:
        void draw();

} ;

#endif