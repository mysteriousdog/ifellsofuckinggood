#ifndef _CONCQUEUE_H_
#define _CONCQUEUE_H_

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

template<typename T>
class ConcQueue
{
public:
    ConcQueue()
    :data(queue<T>()){}
    ConcQueue(const ConcQueue& other) {
        lock_guard<mutex> lock(other.m);
        data = other.data;
    }
    ConcQueue& operator = (const ConcQueue& other)=delete;
    void push(T& new_value) {
        lock_guard<mutex> lock(m);
        data.push(new_value);
        data_cond.notify_one();
    }

    T& front() {
        lock_guard<mutex> lock(m);
        return data.front();
    }

    T pop(){
        lock_guard<mutex> lock(m);
        if (data.empty()) {
            return nullptr;
        }
        T value = data.front();
        data.pop();
        return value;
    }
    bool empty() {
        lock_guard<mutex> lock(m);
        return data.empty();
    }
private:
    queue<T> data;
    mutable mutex m;
    condition_variable data_cond;
};

#endif