#ifndef _CONC_LIST_H_
#define _CONC_LIST_H_

#include <list>
#include <mutex>
#include <condition_variable>
#include <thread>

using namespace std;

template<typename T>
class ConcList
{
public:
    ConcList()
    :data(list<T>()), size(0){}
    ConcList(const ConcList& other) {
        lock_guard<mutex> lock(other.m);
        data = other.data;
    }
    ConcList& operator = (const ConcList& other)=delete;
    void push_back(T& new_value) {
        lock_guard<mutex> lock(m);
        data.push_back(new_value);
        size++;
        data_cond.notify_one();
    }

    void push_front(T& new_value) {
        lock_guard<mutex> lock(m);
        data.push_front(new_value);
        size++;
        data_cond.notify_one();
    }

    T& front() {
        lock_guard<mutex> lock(m);
        return data.front();
    }

    T& back() {
        lock_guard<mutex> lock(m);
        return data.back();
    }

    T pop_back(){
        unique_lock<mutex> lock(m);
        data_cond.wait(lock, [this] {return !(this->data.empty());});
        if (data.empty()) {
            return nullptr;
        }
        T value = data.back();
        data.pop_back();
        size--;
        if (data.empty()) {
            empty_cond.notify_one();
        }
        return value;
    }

    T pop_front(){
        unique_lock<mutex> lock(m);
        data_cond.wait(lock, [this] {return !(this->data.empty());});
        if (data.empty()) {
            return nullptr;
        }
        T value = data.front();
        data.pop_front();
        size--;
        if (data.empty()) {
            empty_cond.notify_one();
        }
        return value;
    }

    bool empty() {
        lock_guard<mutex> lock(m);
        return data.empty();
    }

    bool waitTillNotEmpty(){
        unique_lock<mutex> lock(m);
        data_cond.wait(lock, [this] {return !(this->data.empty());});
        return true;
    }

    void waitPushTillEmpty(T& value) {
        unique_lock<mutex> lock(m);
        empty_cond.wait(lock, [this] {return this->data.empty();});
        // data_cond.wait(lock, [this] {return !(this->data.empty());});
        data.push_back(value);
        data_cond.notify_one();
    }

    void getAllData(list<T>& target) {
        unique_lock<mutex> lock(m);
        for (auto it = data.begin(); it != data.end(); it++) {
            target.push_back(*it);
        }
    }
    
    bool erase(int index) {
        unique_lock<mutex> lock(m);
        if (index < 0 || index >= size) {
            return false;
        }
        auto it = data.begin();
        advance(it, index);
        data.erase(it);
        size--;
        return true;
    }

    int getSize() {
        unique_lock<mutex> lock(m);
        return size;
    }
    bool getOneRequest(int index, T& obj) {
        unique_lock<mutex> lock(m);
        if (index >= 0 && index < size) {
            auto it = data.begin();
            advance(it, index);
            obj = *it;
            return true;
        }
        return false;
    }
private:
    int size;
    list<T> data;
    mutable mutex m;
    condition_variable data_cond;
    condition_variable empty_cond;
};

#endif