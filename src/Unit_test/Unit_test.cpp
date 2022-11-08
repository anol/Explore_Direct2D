//
// Created by aeols on 08.11.2022.
//

#include <iostream>
#include "../Explore_utility.h"

int main() {
    uint16_t midpoint = 80;
    uint16_t steepness = 1;
    uint16_t min_value = 1000;
    uint16_t max_value = 10000;
    for (uint16_t input = 0; input < 160; input++) {
        std::cout << input << ", " <<
                  Explore_utility::logistic(input, midpoint, steepness, min_value, max_value) << std::endl;
    }

    return 0;
}