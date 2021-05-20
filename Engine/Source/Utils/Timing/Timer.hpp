/**
 * From the Cherno
 */

#ifndef GEOGL_TIMER_HPP
#define GEOGL_TIMER_HPP

namespace GEOGL{


    struct ProfileResult{
        const char* Name;
        long long Start, End;
        uint32_t ThreadID;
    };

    struct InstrumentationSession{
        std::string Name;
    };

    class Instrumentor{
    public:
        Instrumentor();

        void beginSession(const std::string& name, const std::string& filepath = "results.json");
        void endSession();

        void writeProfile(const ProfileResult& result);
        void writeHeader();
        void writeFooter();

        static Instrumentor& get();

    private:
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;
    };

    class InstrumentationTimer{
    public:
        InstrumentationTimer(const char* name);
        ~InstrumentationTimer();

        void stop();

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
#define GEOGL_PROFILE_BEGIN_SESSION(name, filepath)     (void(0))
#define GEOGL_PROFILE_END_SESSION()                     (void(0))
#define GEOGL_PROFILE_FUNCTION()                        (void(0))
#define GEOGL_PROFILE_SCOPE(name)                       (void(0))
#endif

#endif //GEOGL_TIMER_HPP
