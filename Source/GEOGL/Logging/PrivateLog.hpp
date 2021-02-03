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


#ifndef VG_ENGINE_PRIVATELOG_HPP
#define VG_ENGINE_PRIVATELOG_HPP

#include "PublicLog.hpp"

// core log macros
#ifdef NDEBUG
//#   define GEOGL_CORE_TRACE
#   define GEOGL_CORE_INFO
#   define GEOGL_CORE_WARN
#   define GEOGL_CORE_ERROR(...)            ::GEOGL::Log::getCoreLogger()->error(__VA_ARGS__)
#   define GEOGL_CORE_CRITICAL(...)         ::GEOGL::Log::getCoreLogger()->critical(__VA_ARGS__)
#else
//#   define GEOGL_CORE_TRACE(...)          ::GEOGL::Log::getCoreLogger()->trace(__VA_ARGS__)
#   define GEOGL_CORE_INFO(...)             ::GEOGL::Log::getCoreLogger()->info(__VA_ARGS__)
#   define GEOGL_CORE_WARN(...)             ::GEOGL::Log::getCoreLogger()->warn(__VA_ARGS__)
#   define GEOGL_CORE_ERROR(...)            ::GEOGL::Log::getCoreLogger()->error(__VA_ARGS__)
#   define GEOGL_CORE_CRITICAL(...)         ::GEOGL::Log::getCoreLogger()->critical(__VA_ARGS__)
#endif
//#define GEOGL_CORE_TRACE_NOSTRIP(...)     ::GEOGL::Log::getCoreLogger()->trace(__VA_ARGS__)
#define GEOGL_CORE_INFO_NOSTRIP(...)        ::GEOGL::Log::getCoreLogger()->info(__VA_ARGS__)
#define GEOGL_CORE_WARN_NOSTRIP(...)        ::GEOGL::Log::getCoreLogger()->warn(__VA_ARGS__)
#define GEOGL_CORE_ERROR_NOSTRIP(...)       ::GEOGL::Log::getCoreLogger()->error(__VA_ARGS__)
#define GEOGL_CORE_CRITICAL_NOSTRIP(...)    ::GEOGL::Log::getCoreLogger()->critical(__VA_ARGS__)

#ifdef GEOGL_ENABLE_ASSERTS
#   define GEOGL_CORE_ASSERT(x, ...) { if(!(x)) { GEOGL_CORE_ERROR_NOSTRIP("Assertion Failed: {0}", __VA_ARGS__); GEOGL_DEBUG_BREAK(); } } (void(0))
#else
#   define GEOGL_CORE_ASSERT(x, ...)
#endif

#endif //VG_ENGINE_PRIVATELOG_HPP
