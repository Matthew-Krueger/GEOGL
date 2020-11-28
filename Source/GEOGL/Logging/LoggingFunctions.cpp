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

#include "PublicLog.hpp"
#include "PrivateLog.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace GEOGL{

    std::shared_ptr<spdlog::logger> Log::coreLogger;
    std::shared_ptr<spdlog::logger> Log::clientLogger;

    void Log::Init(const std::string& filePath, const std::string& clientName) {

        //spdlog::set_pattern("%^[%T] %n: %v%$");
        //coreLogger = spdlog::stdout_color_mt("f");


        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(filePath.c_str());
        file_sink->set_level(spdlog::level::trace);
        file_sink->set_pattern("%^[%c] [%n] [%l] : %v%$");
        //file_sink->set_pattern("%^[%T] %n: %v%$");
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::trace);
        console_sink->set_pattern("%^[%c] [%n] [%l] : %v%$");
        //console_sink->set_pattern("%^[%T] %n: %v%$");

        //spdlog::flush_every(std::chrono::seconds(3));

        coreLogger = std::make_shared<spdlog::logger>(GEOGL_ENGINE_NAME, spdlog::sinks_init_list{console_sink, file_sink});
        clientLogger = std::make_shared<spdlog::logger>(clientName + " Client", spdlog::sinks_init_list{console_sink, file_sink});

        coreLogger->flush_on(spdlog::level::trace);
        clientLogger->flush_on(spdlog::level::trace);

        /*{
            auto core_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            core_console_sink->set_level(spdlog::level::trace);
            core_console_sink->set_pattern("%^[%T] %n: %v%$");

            coreLogger = std::make_shared<spdlog::logger>(VG_ENGINE_NAME, spdlog::sinks_init_list{core_console_sink, file_sink});
        }

        {
            auto client_console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            client_console_sink->set_level(spdlog::level::trace);
            client_console_sink->set_pattern("%^[%T] %n: %v%$");

            clientLogger = std::make_shared<spdlog::logger>("Client", spdlog::sinks_init_list{client_console_sink, file_sink});
        }*/

    }

}
