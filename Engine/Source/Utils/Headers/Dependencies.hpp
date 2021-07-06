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


#ifndef GEOGL_DEPENDENCIES_SOURCE_HPP
#define GEOGL_DEPENDENCIES_SOURCE_HPP

#include "../Memory/TrackMemoryAllocations.hpp"


#include <GEOGL/API_Utils/DLLExportsAndTraps.hpp>

/* JSON */
#include <Nlohmann/json.hpp>
using json = nlohmann::json;

/* STDLIB */
#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <map>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>

/* spdlog */
#include <spdlog/spdlog.h>

/* glm */
#ifdef GEOGL_SWIZZLE
#define GLM_FORCE_SWIZZLE
#endif
#define GLM_FORCE_INTRINSICS
#include <glm/glm.hpp>
#include <glm/simd/matrix.h>
#include <glm/simd/vector_relational.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BIT(x) (1 << x)

#include "../TimeStep.hpp"
#include "../Timing/Timer.hpp"

/* stb libs */
#include <STB/stb_truetype.h>
#include <STB/stb_image.h>




#endif //GEOGL_DEPENDENCIES_HPP