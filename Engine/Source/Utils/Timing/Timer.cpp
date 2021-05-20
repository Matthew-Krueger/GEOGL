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


#include "Timer.hpp"

static const char* stringEscape(const char* toEscape){

    static char buffer[4096];

    strcpy(buffer, toEscape);
    size_t originalIndex = 0;
    size_t originalSize = strlen(toEscape);
    size_t index = 0;
    for(originalIndex = 0; originalIndex<originalSize; ++originalIndex){
        char currentChar = toEscape[originalIndex];

        if(toEscape[originalIndex] != buffer[index]){
            exit(-1);
        }

        if(currentChar == '\"'){
            buffer[index] = '\\';
            ++index;
            buffer[index] = '\0';
            /* now copy the string from this point onwards */
            strcat(&buffer[index],&toEscape[originalIndex]);
        }

        /* don't forget to increment the original index */
        ++index;
    }

    return buffer;

}


namespace GEOGL{

    Instrumentor::Instrumentor()
            : m_CurrentSession(nullptr), m_ProfileCount(0)
    {}

    void Instrumentor::beginSession(const std::string &name, const std::string &filepath){
        m_OutputStream.open(filepath);
        writeHeader();
        m_CurrentSession = new InstrumentationSession{ name };
    }

    void Instrumentor::endSession(){
        writeFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
        m_ProfileCount = 0;
    }

    void Instrumentor::writeProfile(const ProfileResult &result){
        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        const char* name = stringEscape(result.Name);
        m_OutputStream << "{";
        m_OutputStream << R"("cat":"function",)";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << R"("name":")" << name << "\",";
        m_OutputStream << R"("ph":"X",)";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void Instrumentor::writeHeader(){
        m_OutputStream << R"({"otherData": {},"traceEvents":[)";
        m_OutputStream.flush();
    }

    void Instrumentor::writeFooter(){
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

    Instrumentor &Instrumentor::get(){
        static Instrumentor instance;
        return instance;
    }


    InstrumentationTimer::InstrumentationTimer(const char *name)
            : m_Name(name), m_Stopped(false){
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    InstrumentationTimer::~InstrumentationTimer(){
        if (!m_Stopped)
            stop();
    }

    void InstrumentationTimer::stop(){
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        //uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::get().writeProfile({ m_Name, start, end, 0 });

        m_Stopped = true;
    }

}