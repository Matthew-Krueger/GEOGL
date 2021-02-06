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

#include "Callbacks.hpp"

namespace GEOGL{

    void
    windowDefaultDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char *message,
                                   const void *userParam) {

        std::string _source;
        std::string _type;
        std::string _severity;

        switch (source) {
            case GL_DEBUG_SOURCE_API:
                _source = "API";
                break;

            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                _source = "WINDOW SYSTEM";
                break;

            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                _source = "SHADER COMPILER";
                break;

            case GL_DEBUG_SOURCE_THIRD_PARTY:
                _source = "THIRD PARTY";
                break;

            case GL_DEBUG_SOURCE_APPLICATION:
                _source = "APPLICATION";
                break;

            case GL_DEBUG_SOURCE_OTHER:
                _source = "GL UNKNOWN";
                break;

            default:
                _source = "UNKNOWN";
                break;
        }

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                _type = "ERROR";
                break;

            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                _type = "DEPRECATED BEHAVIOR";
                break;

            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                _type = "UDEFINED BEHAVIOR";
                break;

            case GL_DEBUG_TYPE_PORTABILITY:
                _type = "PORTABILITY";
                break;

            case GL_DEBUG_TYPE_PERFORMANCE:
                _type = "PERFORMANCE";
                break;

            case GL_DEBUG_TYPE_OTHER:
                _type = "OTHER";
                break;

            case GL_DEBUG_TYPE_MARKER:
                _type = "MARKER";
                break;

            default:
                _type = "UNKNOWN";
                break;
        }

        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                _severity = "HIGH";
                break;

            case GL_DEBUG_SEVERITY_MEDIUM:
                _severity = "MEDIUM";
                break;

            case GL_DEBUG_SEVERITY_LOW:
                _severity = "LOW";
                break;

            case GL_DEBUG_SEVERITY_NOTIFICATION:
                _severity = "NOTIFICATION";
                // return on release, to avoid the annoying 131185 notification
    #ifdef NDEBUG
                return;
    #else
                break;
    #endif

            default:
                _severity = "UNKNOWN";
                break;
        }

        GEOGL_CORE_ERROR("OpenGL Error {}: {} of {} severity, raised from {}: {}",id, _type, _severity, _source, message);

    }

    void atExitCallback(){
        GEOGL_CORE_INFO_NOSTRIP("Successfully exited GEOGL Engine version {}", GEOGL_VERSION_FULL);
    }

}