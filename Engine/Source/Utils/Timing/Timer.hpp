/**
 * From the Cherno
 */

#ifndef GEOGL_TIMER_HPP
#define GEOGL_TIMER_HPP

namespace GEOGL{


    struct ProfileResult{
        std::string Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession{
        std::string Name;
    };

    class Instrumentor{
    private:
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    public:
        Instrumentor()
                : m_CurrentSession(nullptr), m_ProfileCount(0)
        {}

        void beginSession(const std::string& name, const std::string& filepath = "results.json"){
            m_OutputStream.open(filepath);
            writeHeader();
            m_CurrentSession = new InstrumentationSession{ name };
        }

        void endSession(){
            writeFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        void writeProfile(const ProfileResult& result){
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');

            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            m_OutputStream.flush();
        }

        void writeHeader(){
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        void writeFooter(){
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        static Instrumentor& get(){
            static Instrumentor instance;
            return instance;
        }
    };

    class InstrumentationTimer{
    public:
        InstrumentationTimer(const char* name)
                : m_Name(name), m_Stopped(false){
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~InstrumentationTimer(){
            if (!m_Stopped)
                Stop();
        }

        void Stop(){
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::get().writeProfile({ m_Name, start, end, threadID });

            m_Stopped = true;
        }
    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}

#if GEOGL_BUILD_WITH_PROFILING
#define _GEOGL_PROFILE_TIMER_NAME                       timer
#define GEOGL_PROFILE_BEGIN_SESSION(name, filepath)     ::GEOGL::Instrumentor::get().beginSession(name, filepath)
#define GEOGL_PROFILE_END_SESSION()                     ::GEOGL::Instrumentor::get().endSession()
#define GEOGL_PROFILE_SCOPE(name)                       ::GEOGL::InstrumentationTimer timer##__LINE__(name)
#define GEOGL_PROFILE_FUNCTION()                        GEOGL_PROFILE_SCOPE(__FUNCSIG__)
#else
#define GEOGL_PROFILE_BEGIN_SESSION(name)               (void(0))
#define GEOGL_PROFILE_END_SESSION()                     (void(0))
#define GEOGL_PROFILE_FUNCTION()                        (void(0))
#define GEOGL_PROFILE_SCOPE(name)                       (void(0))
#endif

#endif //GEOGL_TIMER_HPP
