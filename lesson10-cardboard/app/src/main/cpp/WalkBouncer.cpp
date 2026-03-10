//
// Created by monty on 27/02/16.
//
#include <cmath>
#include "WalkBouncer.h"

WalkBouncer::WalkBouncer() : step(0.0f) {

}

void WalkBouncer::tick(long ms) {
	step += ms / 45.0f;
	cached = sin(step) / 20.0f;
}

float WalkBouncer::getCurrentStep() {
	return 0.25f + cached;
}
