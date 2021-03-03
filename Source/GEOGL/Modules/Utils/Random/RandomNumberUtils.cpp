/*******************************************************************************
 * Copyright (c) 2020 Matthew Krueger                                          *
 *                                                                             *
 * This software is provided 'as-is', without any express or implied           *
 * warranty. In no event will the authors be held liable for any damages       *
 * arising from the use of this software.                                      *
 *                                                                             *
 * Permission is granted to anyone to use this software for any purpose,       *
 * including commercial applications, and to alter it and redistribute it      *
 * freely, subject to the following restrictions:                              *
 *                                                                             *
 * 1. The origin of this software must not be misrepresented; you must not     *
 *    claim that you wrote the original software. If you use this software     *
 *    in a product, an acknowledgment in the product documentation would       *
 *    be appreciated but is not required.                                      *
 *                                                                             *
 * 2. Altered source versions must be plainly marked as such, and must not     *
 *    be misrepresented as being the original software.                        *
 *                                                                             *
 * 3. This notice may not be removed or altered from any source                *
 *    distribution.                                                            *
 *                                                                             *
 *******************************************************************************/


#include "RandomNumberUtils.hpp"
#include <random>
namespace GEOGL{

    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<> distributionRNG(1,100'000);
    static std::vector<uint64_t> uuidsGenerated;

    uint64_t generateRandomUUID(){

        uint64_t result = 0;
        while(true) {
            result = distributionRNG(rng);
            auto it = std::find(uuidsGenerated.begin(), uuidsGenerated.end(), result);
            if(it == uuidsGenerated.end() && result != 0)
                break;
        }
        uuidsGenerated.push_back(result);
        return result;

    }

}