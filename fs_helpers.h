#ifndef _FS_HELPERS_H
#define _FS_HELPERS_H

#include "common.h"

long getFileCreationTime(char *filePath)
{
    struct stat attrib;
    stat(filePath, &attrib);
    //char date[20];
    //strftime(date, 20, "%d-%m-%Y", localtime(&(attrib.st_ctime)));
    return (long) mktime(localtime(&(attrib.st_ctime)));
    //strftime(time, 20, "%H:%M:%S", localtime(&(attrib.st_ctime)));
}

void listFiles()
{
    struct dirent* entry;
    DIR* dp; 

    dp = opendir(recording_path.c_str());
    if (NULL == dp) 
    {   
        printf("couldn't open directory\n");
        return;
    }   
    printf("List of files:\n");
    while((entry = readdir(dp)))
    {   
        string str(entry->d_name);
        if (str.find(record_ext) != string::npos)
        {
                string file = recording_path + sep + string(entry->d_name);
                long timestamp = getFileCreationTime((char*)file.c_str());
                printf("%s, ts=%lu\n", file.c_str(), timestamp);
        }   
    }   
    printf("-----\n");

    closedir(dp);
}

bool checkDir()
{
    struct stat sb;
    if (stat(recording_path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        //printf("is directory and exists\n");
	return true;
    }
    return false;
}

bool dirContainsRecording()
{
    if (!checkDir())
    {
        return false;
    }
    struct dirent* entry;

    DIR* dp; 
//printf("%s\n", recording_path.c_str());
    dp = opendir(recording_path.c_str());

    if (NULL == dp) 
    {
        return 0;
    }   
//printf("aaaa\n");
    int num = 0;

    while((entry = readdir(dp)))
    {   
        string str(entry->d_name);
        if (str.find(record_ext) != string::npos)
        {
	    num++;
        }   
    }

    closedir(dp);

    return (num > 0);
}
void checkStorageUsage()
{
    system(check_storage_usage.c_str());
//printf("%s\n", check_storage_usage.c_str());
//printf("%s\n", storage_usage_result.c_str());

    fstream file(storage_usage_result.c_str());
    string line;
    getline(file, line);

    string ssize = line.substr(0, line.length() - 1);
    long converted = atoi(ssize.c_str());
    if (converted > 98)
    {
        sendMailRecordingFail(storage_usage_warning);
    }
}

int getAllRecordingSizes()
{
    system(check_file_sizes.c_str());
    fstream file(sizes_result.c_str());
    string line;

    getline(file, line);
    file.close();
    return atoi(line.c_str());
}

#endif
