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

#include "Core.hpp"

#ifndef GEOGL_MAINCREATOR_HPP
#define GEOGL_MAINCREATOR_HPP

int main(int argc, char ** argv);

#ifdef GEOGL_INCLUDE_WIN_MAIN
#   ifdef WIN32
#       ifdef APIENTRY
#           undef APIENTRY
#       endif
#   include <windows.h>
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

#   endif

#endif

/* Required external */
extern GEOGL::Application* GEOGL::createApplication();

#ifdef GEOGL_INCLUDE_MAIN
/* Actual Main function */
int main(int argc, char ** argv){

    GEOGL::Application* application = GEOGL::createApplication();
    application->run();
    delete application;

}
#endif
#endif //GEOGL_MAINCREATOR_HPP
