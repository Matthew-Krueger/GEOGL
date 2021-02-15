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


#ifndef NODIFY_SCREENWRITER_CALLBACKSPRIVATE_HPP
#define NODIFY_SCREENWRITER_CALLBACKSPRIVATE_HPP

/**
 * \brief Provides a callback for OpenGL errors, and uses severe error log
 * @param source OpenGL Provided Value
 * @param type OpenGL Provided Value
 * @param id OpenGL Provided Value
 * @param severity OpenGL Provided Value
 * @param length OpenGL Provided Value
 * @param message OpenGL Provided Value
 * @param userParam OpenGL Provided Value
 */
GEOGL_API void windowDefaultDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                          const char *message, const void *userParam);


#endif //NODIFY_SCREENWRITER_CALLBACKSPRIVATE_HPP