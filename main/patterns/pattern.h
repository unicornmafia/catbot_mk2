#ifndef __PATTERN_H__
#define __PATTERN_H__

static char tag[] = "catbot_laser_servos";


class Pattern {

    public:
        int m_min_value = 1200;
        int m_max_value = 1700;
        int m_duty_x = 1500;
        int m_duty_y = 1500;
        Pattern();
        virtual void draw() = 0;
};

#endif