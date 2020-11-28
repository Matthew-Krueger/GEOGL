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

#ifndef GEOGL_CORE_HPP
#define GEOGL_CORE_HPP

#ifdef INCLUDE_WIN_MAIN

int main(int argc, char ** argv);
#ifdef WIN32

#ifdef APIENTRY
#undef APIENTRY
#endif
#include <windows.h>
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPWSTR, INT)
{

    INT returnCode;
    UNREFERENCED_PARAMETER(hInst);
    UNREFERENCED_PARAMETER(hPrevInstance);

    int argc;
    char** argv;
    {
        LPWSTR* lpArgv = CommandLineToArgvW( GetCommandLineW(), &argc );
        argv = (char**) malloc( argc*sizeof(char*) );
        int size, i = 0;
        for( ; i < argc; ++i ) {
            size = wcslen( lpArgv[i] ) + 1;
            argv[i] = (char*) malloc( size );
            wcstombs( argv[i], lpArgv[i], size );
        }

        returnCode = (INT) main(argc, argv);

        LocalFree( lpArgv );
    }

    return returnCode;

}

#endif

#endif

#include "../Win32Exports.hpp"

#include <string>
#include <vector>
#include <memory>
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vector_relational.hpp>

/* Stb Image */
#include <STB/stb_image.h>

/* GLAD */
#include <glad/glad.h>

/* GLFW */
#include <GLFW/glfw3.h>

#include "../../ModelComponents/RawModel.hpp"

#include "../../Logging/PublicLog.hpp"

#include "../../Utils/Callbacks.hpp"
#include "../../Rendering/GraphicsInstance.hpp"
#include "../../Utils/Loader.hpp"

#include "../../Rendering/Renderer.hpp"


#endif //GEOGL_CORE_HPP
