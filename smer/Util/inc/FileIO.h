#ifndef _FILE_IO_H_
#define _FILE_IO_H_
#include <boost/filesystem.hpp>
#include "Singleton.h"
#include <iostream>
#include <map>
using namespace std;
using namespace boost::filesystem;


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

private:
    map<string, boost::filesystem::fstream*> path2file;

friend class Singleton;
};


#endif