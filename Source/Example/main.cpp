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
#ifdef WIN32
#define INCLUDE_WIN_MAIN
#endif
#include <GEOGL/Core.hpp>


int main(int argc, char ** argv){

    GEOGL::Log::Init("log.txt", "Example");

    GEOGL_INFO("Creating Graphics instance. CMD argument count {}", argc);
    auto gi = GEOGL::GraphicsInstance::makeGraphicsInstance(1280,720,"Window You");

    GEOGL_INFO_NOSTRIP("Loading quad");
    auto rm = GEOGL::Loader::loadToVAO({ -0.5f, 0.5f, 0,
                                         -0.5f, -0.5f, 0,
                                         0.5f, -0.5f, 0,
                                         0.5f, -0.5f, 0,
                                         0.5f, 0.5f, 0,
                                         -0.5f, 0.5f, 0});
    GEOGL_INFO_NOSTRIP("Loaded quad");


    GEOGL::Renderer r;

    GEOGL_INFO_NOSTRIP("Starting Loop");
    while(!gi->windowShouldClose()){
        r.prepare();

        r.render(*rm);

        gi->updateDisplay();

    }

    return 0;

}