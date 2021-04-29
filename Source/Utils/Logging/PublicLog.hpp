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

/* This file is also used for private log, but I don't care, so, it goes here. */


#ifndef GEOGL_PUBLICLOG_HPP
#define GEOGL_PUBLICLOG_HPP

#include "../Headers/Refs.hpp"
#include <spdlog/spdlog.h>

namespace GEOGL{


    /**
     * \brief Represents the internals of the logging API.
     */
    class GEOGL_API Log{
    private:
        static Ref<spdlog::logger> coreLogger;
        static Ref<spdlog::logger> clientLogger;

    public:
        static void Init(const std::string& filePath, const std::string& clientName = "Client");

        inline static Ref<spdlog::logger>& getCoreLogger(){ return coreLogger; }
        inline static Ref<spdlog::logger>& getClientLogger(){ return clientLogger; }

    };

}

// client log macros
#define GEOGL_FORMAT(...)           ::fmt::format(__VA_ARGS__)
#ifdef NDEBUG
//#   define GEOGL_TRACE
#   define GEOGL_INFO
#   define GEOGL_WARN
#   define GEOGL_ERROR(...)         ::GEOGL::Log::getClientLogger()->error(__VA_ARGS__)
#   define GEOGL_CRITICAL(...)      ::GEOGL::Log::getClientLogger()->critical(__VA_ARGS__)
#else
/**
 * Enables asserts when compiling in debug mode
 */
#   define GEOGL_ENABLE_ASSERTS
#   define GEOGL_TRACE(...)       ::GEOGL::Log::getClientLogger()->trace(__VA_ARGS__)
#   define GEOGL_INFO(...)          ::GEOGL::Log::getClientLogger()->info(__VA_ARGS__)
#   define GEOGL_WARN(...)          ::GEOGL::Log::getClientLogger()->warn(__VA_ARGS__)
#   define GEOGL_ERROR(...)         ::GEOGL::Log::getClientLogger()->error(__VA_ARGS__)
#   define GEOGL_CRITICAL(...)      ::GEOGL::Log::getClientLogger()->critical(__VA_ARGS__)
#endif

//#define GEOGL_TRACE_NOSTRIP(...)  ::GEOGL::Log::getClientLogger()->trace(__VA_ARGS__)
#define GEOGL_INFO_NOSTRIP(...)     ::GEOGL::Log::getClientLogger()->info(__VA_ARGS__)
#define GEOGL_WARN_NOSTRIP(...)     ::GEOGL::Log::getClientLogger()->warn(__VA_ARGS__)
#define GEOGL_ERROR_NOSTRIP(...)    ::GEOGL::Log::getClientLogger()->error(__VA_ARGS__)
#define GEOGL_FATAL_CRITICAL(...)   ::GEOGL::Log::getClientLogger()->critical(__VA_ARGS__)

#ifdef GEOGL_ENABLE_ASSERTS
#   define GEOGL_ASSERT(x, ...) { if(!(x)) { GEOGL_ERROR_NOSTRIP("Assertion Failed: {0}", GEOGL_FORMAT(__VA_ARGS__)); GEOGL_DEBUG_BREAK(); } } (void(0))
#else
#   define GEOGL_ASSERT(x, ...) { x; } (void(0))
#endif
/**
 * Forms a non-stripping assert. If not launched with a debugger, this causes a hard crash
 */
#define GEOGL_ASSERT_NOSTRIP(x, ...) { if(!(x)) { GEOGL_ERROR_NOSTRIP("Assertion Failed: {0}", GEOGL_FORMAT(__VA_ARGS__)); GEOGL_DEBUG_BREAK(); } } (void(0))

#endif //GEOGL_PUBLICLOG_HPP