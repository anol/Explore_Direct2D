//
// Created by aeols on 08.11.2022.
//

#ifndef EXPLORE_DIRECT2D_EXPLORE_UTILITY_H
#define EXPLORE_DIRECT2D_EXPLORE_UTILITY_H

#include <cstdint>

class Explore_utility {

public:
    static uint16_t logistic(uint16_t input, uint16_t midpoint, uint16_t steepness,
                             uint16_t min_value, uint16_t max_value);

};


#endif //EXPLORE_DIRECT2D_EXPLORE_UTILITY_H
