/**
 * From the Cherno
 */

#ifndef GEOGL_TIMER_HPP
#define GEOGL_TIMER_HPP

namespace GEOGL{

    GEOGL_API const char* cStringEscape(const char* toEscape);

    struct ProfileResult{
        const char* Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession{
        std::string Name;
    };

    class GEOGL_API Instrumentor{
    public:
        Instrumentor();

        void beginSession(const std::string& name, const std::string& filepath = "results.json");
        void endSession();

        inline void writeProfile(const ProfileResult &result){
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            const char* name = cStringEscape(result.Name);
            m_OutputStream << "{";
            m_OutputStream << R"("cat":"function",)";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
            m_OutputStream << R"("name":")" << name << "\",";
            m_OutputStream << R"("ph":"X",)";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            //m_OutputStream.flush();
        }
        void writeHeader();
        void writeFooter();

        inline static Instrumentor& get(){
            static Instrumentor instance;
            return instance;
        }

    private:
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    };

    class GEOGL_API InstrumentationTimer{
    public:
        inline InstrumentationTimer(const char* name)
                : m_Name(name), m_Stopped(false){
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
            m_StartTime = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            static long long previousTime = 0;
            if(m_StartTime == previousTime) {
                m_StartTime++;
            }
            previousTime = m_StartTime;
        }
        inline ~InstrumentationTimer(){
            if (!m_Stopped)
                stop();
        }

        inline void stop(){
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            //long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::get().writeProfile({ m_Name, m_StartTime, end, threadID });

            m_Stopped = true;
        }

    private:
        const char* m_Name;
        long long m_StartTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}

#if GEOGL_BUILD_WITH_PROFILING
#ifdef __GNUC__
#define _GEOGL_PROFILE_FUNCTION_NAME                    __PRETTY_FUNCTION__
#else
#define _GEOGL_PROFILE_FUNCTION_NAME                    __FUNCSIG__
#endif
#define _GEOGL_PROFILE_TIMER_NAME                       timer
#define GEOGL_PROFILE_BEGIN_SESSION(name, filepath)     ::GEOGL::Instrumentor::get().beginSession(name, filepath)
#define GEOGL_PROFILE_END_SESSION()                     ::GEOGL::Instrumentor::get().endSession()
#define GEOGL_PROFILE_SCOPE(name)                       ::GEOGL::InstrumentationTimer timer##__LINE__(name)
#define GEOGL_PROFILE_FUNCTION()                        GEOGL_PROFILE_SCOPE(_GEOGL_PROFILE_FUNCTION_NAME)
#else
#define GEOGL_PROFILE_BEGIN_SESSION(name, filepath)     (void(0))
#define GEOGL_PROFILE_END_SESSION()                     (void(0))
#define GEOGL_PROFILE_FUNCTION()                        (void(0))
#define GEOGL_PROFILE_SCOPE(name)                       (void(0))
#endif

#if GEOGL_BUILD_WITH_RENDERER_PROFILING
#define GEOGL_RENDERER_PROFILE_FUNCTION()   GEOGL_PROFILE_FUNCTION()
#define GEOGL_RENDERER_PROFILE_SCOPE(name)  GEOGL_PROFILE_SCOPE(name)
#else
#define GEOGL_RENDERER_PROFILE_FUNCTION()
#define GEOGL_RENDERER_PROFILE_SCOPE(name)
#endif


#endif //GEOGL_TIMER_HPP
