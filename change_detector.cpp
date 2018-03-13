#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include "common.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
using namespace std;
//some txt
std::string sh_exec(string str) {
    const char* cmd = str.c_str();
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

int is_update_required()
{
    string commit_timestamp = "git --git-dir "+prog_path+".git log -1 --format=%ct";
    string pull_changes = "git --git-dir "+prog_path+".git pull origin master";
    
    //cout << "## " << commit_timestamp << endl;
    //cout << "## " << pull_changes << endl;
    
    string start_ts = sh_exec(commit_timestamp);
    sh_exec(pull_changes);
    string end_ts = sh_exec(commit_timestamp);

    return (start_ts != end_ts);
}

class RecordingDetectorProcesManager
{
    public:
        void start_camman()
        {
            pid = fork();
            if (pid == 0)
            {
                char prog_name[10] = "./camman";
                char* args[] = {prog_name, NULL};
                execvp(args[0], args);
            }
        }
        void stop_camman()
        {
            kill(pid, SIGINT);
            sleepSec(1);
            wait(&pid);
        }

        void rebuild_camman()
        {
            const char* rebuild_camera_manager = "make clean_camman; make camman";
            system(rebuild_camera_manager);
        }
    private:
        int pid;
};

int main()
{
    RecordingDetectorProcesManager manager;
    manager.start_camman();
    
    while(1)
    {
        if (is_update_required())
        {
            manager.stop_camman();
            manager.rebuild_camman();
            manager.start_camman();
        }
        sleepSec(5);
    }   
    return 0;
}
