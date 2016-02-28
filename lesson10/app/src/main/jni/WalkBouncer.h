//
// Created by monty on 27/02/16.
//

#ifndef LESSON10_WALKBOUNCER_H
#define LESSON10_WALKBOUNCER_H


class WalkBouncer {
    float step;
    float cached;
public:
    WalkBouncer();

    void tick(long ms);

    float getCurrentStep();
};


#endif //LESSON10_WALKBOUNCER_H
