#ifndef _FILE_IO_H_
#define _FILE_IO_H_
#include <boost/filesystem.hpp>
#include "Singleton.h"
#include <iostream>
#include <map>
using namespace std;
using namespace boost::filesystem;

// std::string path = "wwww1.txt";
 
    // history history1;
    // history1.m_id = 20;
    // history1.m_kk = 20;
    // history1.m_sec = 20;
    // history1.name = "abacdefasdfasdfwerwer";
 
    // //file path
    // boost::filesystem::path fpath(path);
    // boost::filesystem::fstream fstream(fpath, std::ios_base::out);
 
    // //write data to file
    // fstream << history1;
    // fstream.close();
 
 
    // boost::filesystem::fstream fstream1(fpath, std::ios_base::in);
    // history history2;
    // //read data from file
    // fstream1 >> history2;
 
    // std::cout << history2.m_id << std::endl;
    // std::cout << history2.name << std::endl;
 
 
    // fstream1.close();


class FileIo : public Singleton<FileIo>
{

public:
    boost::filesystem::fstream* getOutFileByPath(const string& path) {
        boost::filesystem::path fpath(path);
        boost::filesystem::fstream* f = new boost::filesystem::fstream(fpath, std::ios_base::app);
        path2file[path] = f;
        return f;
    }

    void closeFile(const string& path) {
        if (path2file.count(path) > 0) {
            auto f = path2file[path];
            (*f).close();
            path2file[path] = nullptr;
        }
    }

    bool writeToFile(const string& path, const string& str) {

        if (path2file.count(path) > 0 && path2file[path] != nullptr) {
            auto f = path2file[path];
            (*f) << str;
        } else {
            boost::filesystem::path fpath(path);
            boost::filesystem::fstream* f = new boost::filesystem::fstream(fpath, std::ios_base::app);
            path2file[path] = f;
            (*f) << str;
            (*f).flush();
            // (*f).close();
        }
    }
    ~FileIo() {
        for (auto it = path2file.begin(); it != path2file.end(); it++) {
            if (it->second == nullptr) {
                continue;
            }
            (*(it->second)).close();
        }
    }

    // bool writeToFile(const string& path, const string& str) {
    //     auto f = getOutFileByPath(path);
    //     f << str;
    // }

private:
    map<string, boost::filesystem::fstream*> path2file;

friend class Singleton;
};


#endif