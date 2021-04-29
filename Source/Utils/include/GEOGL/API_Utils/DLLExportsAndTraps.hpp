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

#ifndef GEOGL_WIN32EXPORTS
#define GEOGL_WIN32EXPORTS

#ifndef GEOGL_BUILD_STATIC
#if defined _WIN32 || defined __CYGWIN__
#   ifdef GEOGL_BUILD_LIBRARY
// Exporting...
#       ifdef __GNUC__
#           define GEOGL_API __attribute__ ((dllexport))
#       else
#           define GEOGL_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#       endif
#   else
#       ifdef __GNUC__
#           define GEOGL_API __attribute__ ((dllimport))
#       else
#           define GEOGL_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#       endif
#   endif
#   define GEOGL_API_HIDDEN
#else
#   if __GNUC__ >= 4
#       define GEOGL_API __attribute__ ((visibility ("default")))
#       define GEOGL_API_HIDDEN  __attribute__ ((visibility ("hidden")))
#   else
#       define GEOGL_API
#       define GEOGL_API_HIDDEN
#       error Problem configuring
#   endif
#endif
#else
#define GEOGL_API
#define GEOGL_API_HIDDEN
#endif

/* Debugging assertions and traps
 * Portable Snippets - https://github.com/nemequ/portable-snippets
 * Created by Evan Nemerson <evan@nemerson.com>
 *
 *   To the extent possible under law, the authors have waived all
 *   copyright and related or neighboring rights to this code.  For
 *   details, see the Creative Commons Zero 1.0 Universal license at
 *   https://creativecommons.org/publicdomain/zero/1.0/
 */

#if !defined(PSNIP_NDEBUG) && defined(NDEBUG) && !defined(PSNIP_DEBUG)
#  define PSNIP_NDEBUG 1
#endif

#if defined(__has_builtin) && !defined(__ibmxl__)
#  if __has_builtin(__builtin_debugtrap)
#    define GEOGL_DEBUG_BREAK() __builtin_debugtrap()
#  elif __has_builtin(__debugbreak)
#    define GEOGL_DEBUG_BREAK() __debugbreak()
#  endif
#endif
#if !defined(GEOGL_DEBUG_BREAK)
#  if defined(_MSC_VER) || defined(__INTEL_COMPILER)
#    define GEOGL_DEBUG_BREAK() __debugbreak()
#  elif defined(__ARMCC_VERSION)
#    define GEOGL_DEBUG_BREAK() __breakpoint(42)
#  elif defined(__ibmxl__) || defined(__xlC__)
#    include <builtins.h>
#    define GEOGL_DEBUG_BREAK() __trap(42)
#  elif defined(__DMC__) && defined(_M_IX86)
static inline void GEOGL_DEBUG_BREAK(void) { __asm int 3h; }
#  elif defined(__i386__) || defined(__x86_64__)
static inline void GEOGL_DEBUG_BREAK(void) { __asm__ __volatile__("int3"); }
#  elif defined(__thumb__)
static inline void GEOGL_DEBUG_BREAK(void) { __asm__ __volatile__(".inst 0xde01"); }
#  elif defined(__aarch64__)
     static inline void GEOGL_DEBUG_BREAK(void) { __asm__ __volatile__(".inst 0xd4200000"); }
#  elif defined(__arm__)
     static inline void GEOGL_DEBUG_BREAK(void) { __asm__ __volatile__(".inst 0xe7f001f0"); }
#  elif defined (__alpha__) && !defined(__osf__)
     static inline void GEOGL_DEBUG_BREAK(void) { __asm__ __volatile__("bpt"); }
#  elif defined(_54_)
     static inline void GEOGL_DEBUG_BREAK(void) { __asm__ __volatile__("ESTOP"); }
#  elif defined(_55_)
     static inline void GEOGL_DEBUG_BREAK(void) { __asm__ __volatile__(";\n .if (.MNEMONIC)\n ESTOP_1\n .else\n ESTOP_1()\n .endif\n NOP"); }
#  elif defined(_64P_)
     static inline void GEOGL_DEBUG_BREAK(void) { __asm__ __volatile__("SWBP 0"); }
#  elif defined(_6x_)
     static inline void GEOGL_DEBUG_BREAK(void) { __asm__ __volatile__("NOP\n .word 0x10000000"); }
#  elif defined(__STDC_HOSTED__) && (__STDC_HOSTED__ == 0) && defined(__GNUC__)
#    define GEOGL_DEBUG_BREAK() __builtin_trap()
#  else
#    include <signal.h>
#    if defined(SIGTRAP)
#      define GEOGL_DEBUG_BREAK() raise(SIGTRAP)
#    else
#      define GEOGL_DEBUG_BREAK() raise(SIGABRT)
#    endif
#  endif
#endif

#endif //GEOGL_WIN32EXPORTS