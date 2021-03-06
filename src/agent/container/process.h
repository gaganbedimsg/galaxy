// Copyright (c) 2016, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once
#include "util/error_code.h"
#include <stdint.h>

#include <boost/function.hpp>

#include <string>
#include <map>
#include <vector>

#ifndef CLONE_NEWPID
#define CLONE_NEWPID 0x20000000
#endif

#ifndef CLONE_NEWUTS
#define CLONE_NEWUTS 0x04000000
#endif

/*
#define __LOG_STDERROR__(format, ...) do { \
 fprintf(stderr, "%s:%d"##format, __FILE__, __LINE__,  ##args); \
} while(0)

#define __LOG_STDOUT__(format, args, ...) do { \
 fprintf(stdout, format, ##args); \
} while(0)
*/

namespace baidu {
namespace galaxy {
namespace container {

class Process {
public:
    Process();
    ~Process();

    // return pid of call process
    static pid_t SelfPid();
    static baidu::galaxy::util::ErrorCode Kill(pid_t pid);

    int RedirectStderr(const std::string& path);
    int RedirectStdout(const std::string& path);

    int Clone(boost::function<int (void*)> routine, void* param, int32_t flag);
    int Fork(boost::function<int (void*)> routine, void* param);
    int Wait(int& status);
    void Reload(pid_t pid);
    pid_t Pid();

private:
    class Context {
    public:
        Context() : self(NULL),
            stdout_fd(-1),
            stderr_fd(-1),
            routine(NULL),
            parameter(NULL) {
        }

    public:
        Process* self;
        int stdout_fd;
        int stderr_fd;
        boost::function<int (void*)> routine;
        void* parameter;
        std::vector<int> fds;
    };

    static int CloneRoutine(void* self);

    int ListFds(pid_t pid, std::vector<int>& fd);

    pid_t pid_;

    std::string user_;
    std::string _user_group;
    std::string stderr_path_;
    std::string stdout_path_;

};

} //namespace container
} //namespace galaxy
} //namespace baidu
