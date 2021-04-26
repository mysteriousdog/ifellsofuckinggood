#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template<class T>
class Singleton
{
public:
    static T& getInstance() {
        static T singleton;
        return singleton;
    }
    virtual ~Singleton(){};
protected:
    Singleton(){};
private:
  Singleton(const Singleton& s);
  Singleton& operator = (Singleton& s);

};

#define GET_SINGLE(name) #name->getInstance();

#endif