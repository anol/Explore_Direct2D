//
// Created by aeols on 08.11.2022.
//

#include <cmath>

#include "Explore_utility.h"

uint16_t Explore_utility::logistic(uint16_t input, uint16_t midpoint, double steepness,
                                   uint16_t min_value, uint16_t max_value) {
    auto result = (max_value - min_value) / (1 + exp(-steepness * (input - midpoint)));
    uint16_t natural_number = min_value + (uint16_t) round(result);
    return natural_number;
}
