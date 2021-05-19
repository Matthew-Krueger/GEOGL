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

#include "TrackMemoryAllocations.hpp"

static size_t bytesAllocated = 0;
static size_t allocations = 0;
static size_t bytesDeallocated = 0;
static size_t deallocations = 0;

size_t GEOGL::getNumberAllocations() {
    return allocations;
}

size_t GEOGL::getNumberDeallocations() {
    return deallocations;
}

size_t GEOGL::getBytesAllocated() {
    return bytesAllocated;
}

double GEOGL::getKilobytesAllocated() {
    return (double)bytesAllocated/(double)1024;
}

double GEOGL::getMegabytesAllocated() {
    return (double)bytesAllocated/(double)(1024*2);
}

double GEOGL::getKilobytesDeallocated(){
    return (double)bytesDeallocated/(double)(1024);
}

double GEOGL::getMegabytesDeallocated(){
    return (double)bytesDeallocated/(double)(1024*2);
}


#if (GEOGL_TRACK_MEMORY_ALLOC_FLAG == 1)
void* operator new(size_t bytesToAllocate){
    ++allocations;
    bytesAllocated+=bytesToAllocate;
    return malloc(bytesToAllocate);
}
void* operator new[](size_t bytesToAllocate){
    ++allocations;
    bytesAllocated+=bytesToAllocate;
    return calloc(1,bytesToAllocate);
}
void operator delete(void* ptrToDealloc, size_t size) noexcept{
    ++deallocations;

    bytesDeallocated+= size;

    free(ptrToDealloc);
}

void operator delete[](void *ptrToDealloc, size_t size) noexcept{
    ++deallocations;

    bytesDeallocated+= size;

    free(ptrToDealloc);
}

#endif