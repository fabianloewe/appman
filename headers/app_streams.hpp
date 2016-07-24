#ifndef __APPLICATION_STREAMS__
#define __APPLICATION_STREAMS__

#include <ostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <mutex>
#include <thread>

#include "app.hpp"
#include "app_task_prio.hpp"

namespace hyronx
{
namespace appman
{
    
class Application;

class ApplicationStream : private std::ostream
{
public:
    ApplicationStream() = default;
    inline ApplicationStream(std::streambuf *buf, bool main_thread_last = false, std::thread::id main_thread = std::thread::id()) :
        std::ostream(buf),
        as_main_thread(main_thread)
    {
    }
    
    inline auto getApplication() const noexcept -> const Application * const
    {
        return as_app;
    }
    
    inline void setApplication(const Application * const app)
    {
        as_app = app;
    }
    
    inline auto getStreamBuf() const -> std::streambuf *
    {
        return rdbuf();
    }
    
    inline void registerMainThread(std::thread::id main_thread) 
    {
        as_sched.registerThread({main_thread, TaskPriority::LOW});
    }
    
    inline void registerThread(std::thread::id thread) 
    {
        as_sched.registerThread({thread, TaskPriority::MIDDLE});
    }
    
    inline void connectTo(std::ostream &out)
    {
        out.tie(this);
    }
    
    template<typename T>
    friend auto operator <<(ApplicationStream &, const T &data) -> ApplicationStream &;
    
    friend auto operator <<(ApplicationStream &out, std::ostream &(*func)(std::ostream &)) -> ApplicationStream &;
    
private:
    const Application *as_app;
    std::thread::id as_main_thread;
    static ApplicationTaskScheduler as_sched;
};

extern ApplicationStream cout;

template<typename T>
inline auto operator <<(ApplicationStream &out, const T &data) -> ApplicationStream &
{
    auto id = std::this_thread::get_id();
    if(id != out.as_main_thread)
        out.registerThread(id);
    
    out.as_sched.lock();
    static_cast<std::ostream &>(out) << out.as_app->getName() << " : " << data;
    out.as_sched.unlock();        
    
    return out;    
}

inline auto operator <<(ApplicationStream &out, std::ostream &(*func)(std::ostream &)) -> ApplicationStream &
{
    auto id = std::this_thread::get_id();
    if(id != out.as_main_thread)
        out.registerThread(id);
    
    out.as_sched.lock();        
    static_cast<std::ostream &>(out) << func;
    out.as_sched.unlock();
    
    return out;
}

}
}

#endif