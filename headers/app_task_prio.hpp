#ifndef __APPLICATION_TASK_PRIORITY__
#define __APPLICATION_TASK_PRIORITY__

#include <map>
#include <thread>
#include <mutex>
#include <utility>

namespace hyronx
{
namespace appman
{
    
enum class TaskPriority
{
    NONE,
    LOW,
    MIDDLE,
    HIGH
};

// This class must be statically used otherwise interfer the mutexes with each other
class ApplicationTaskScheduler
{
public:    
    void registerThread(std::pair<std::thread::id, TaskPriority> thread_pair)
    {
        at_threads.insert(thread_pair);
    }
    
    void lock(std::thread::id thread_id = std::this_thread::get_id())
    {
        TaskPriority prio = TaskPriority::NONE;
        if(thread_id == at_current_thread.first)
            prio = at_current_thread.second;
        else
        {
            auto it = at_threads.find(thread_id);
            if(it == at_threads.end())
            {
                lockLowPriorized();
                return;  
            }
            
            prio = it->second;              
            at_current_thread = *it;
        } 
        
        if(prio == TaskPriority::NONE || prio == TaskPriority::LOW)
            lockLowPriorized();
        else if(prio == TaskPriority::MIDDLE)
            lockMiddlePriorized();
        else
            lockHighPriorized();      
    }
    
    void unlock(std::thread::id thread_id = std::this_thread::get_id())
    {
        TaskPriority prio = TaskPriority::NONE;
        if(thread_id == at_current_thread.first)
            prio = at_current_thread.second;
        else
        {
            auto it = at_threads.find(thread_id);
            if(it == at_threads.end())
            {
                unlockLowPriorized();
                return;  
            }
            
            prio = it->second;              
            at_current_thread = *it;
        } 
        
        if(prio == TaskPriority::NONE || prio == TaskPriority::LOW)
            unlockLowPriorized();
        else if(prio == TaskPriority::MIDDLE)
            unlockMiddlePriorized();
        else
            unlockHighPriorized();      
    }
    
private:
    std::map<std::thread::id, TaskPriority> at_threads;  
    std::pair<std::thread::id, TaskPriority> at_current_thread;  
    std::mutex at_da;   // data access mutex
    std::mutex at_nda;  // next-to-access mutex
    std::mutex at_m;    // middle-priority mutex
    std::mutex at_l;    // low-priority mutex    
    
private:
    void lockLowPriorized()
    {
        at_l.lock();
        at_m.lock();
        at_nda.lock();
        at_da.lock();
        at_nda.unlock();
    }
    
    void lockMiddlePriorized()
    {
        at_m.lock();
        at_nda.lock();
        at_da.lock();
        at_nda.unlock();
    }
    
    void lockHighPriorized()
    {
        at_nda.lock();
        at_da.lock();
        at_nda.unlock();
    }
    
    void unlockLowPriorized()
    {
        at_da.unlock();
        at_m.unlock();
        at_l.unlock();
    }
    
    void unlockMiddlePriorized()
    {
        at_da.unlock();
        at_m.unlock();
    }
    
    void unlockHighPriorized()
    {
        at_da.unlock();
    }
};
    
}
}


#endif