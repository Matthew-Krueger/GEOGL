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


#ifndef GEOGL_TRACKMEMORYALLOCATIONS_HPP
#define GEOGL_TRACKMEMORYALLOCATIONS_HPP

/* This file is included in the PCH. This file will be used in conjunction with cmake to either
 * strip out memory allocations or track memory allocations */

/* Due to operator new restrictions, we cannot really namespace this. The getters will
 * be namespaced though */


#include <cstddef>
#include <GEOGL/API_Utils/DLLExportsAndTraps.hpp>
namespace GEOGL{

    GEOGL_API size_t getNumberAllocations();
    GEOGL_API size_t getNumberDeallocations();
    GEOGL_API size_t getBytesAllocated();

    GEOGL_API double getKilobytesAllocated();
    GEOGL_API double getMegabytesAllocated();

}

#if (GEOGL_TRACK_MEMORY_ALLOC_FLAG == 1)
GEOGL_API void* operator new(size_t bytesToAllocate);
GEOGL_API void* operator new[](size_t bytesToAllocate);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-exception-spec-mismatch"
GEOGL_API void operator delete(void* ptrToDealloc);
GEOGL_API void operator delete[](void* ptrToDealloc);
#pragma clang diagnostic pop
#endif

#endif //GEOGL_TRACKMEMORYALLOCATIONS_HPP
