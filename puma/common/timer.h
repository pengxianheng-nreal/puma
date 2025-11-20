#pragma once

#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>
class Timer{
public:
    Timer() 
    :running_(false){ }
 
    Timer(const Timer& t){
        running_ = t.running_.load();
    }
    ~Timer(){
        Expire();
    }
 
    void StartTimer(int32_t interval, std::function<void()> task){
        if (running_){ return; }
        running_ = true;
        work_thread_ = std::thread([this, interval, task](){
            while (running_){
                auto delta = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
                task();
                std::this_thread::sleep_until(delta);
            }
        });
    }
 
    void Expire(){
        running_ = false;
        if (work_thread_.joinable()) {
            if (work_thread_.get_id() != std::this_thread::get_id()) {
                work_thread_.join();
            } else {
                work_thread_.detach();
            }
        }
    }
     
    template<typename callable, class... arguments>
    void SyncWait(int32_t after, callable&& f, arguments&&... args){ 
        std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }

    template<typename callable, class... arguments>
    void AsyncWait(int32_t after, callable&& f, arguments&&... args){
        std::function<typename std::result_of<callable(arguments...)>::type()> task
            (std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));
 
        std::thread([after, task](){
            std::this_thread::sleep_for(std::chrono::milliseconds(after));
            task();
        }).detach();
    }
     
private:
    std::atomic<bool> running_;
    std::thread work_thread_;
};