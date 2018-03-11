#include "common.h"
#include "mail_sender.h"
#include "fs_helpers.h"

void restartFtp()
{
    system(restart_ftp.c_str());
    int status = system(ss1.c_str());
    
        sendMailRecordingFail(no_ftp_restart + "-stop");
    if (status != 0)
    {
        sendMailRecordingFail(no_ftp_restart + "-stop");
    }
    status = system(ss2.c_str());

    //printf("returned status = %d\n", status);
    if (status != 0)
    {
        sendMailRecordingFail(no_ftp_restart + "-start");
    }
}

void sleepHour()
{
    sleepSec(60 * 60);
}

bool currentTimeBetween2and3AM()
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    return (now->tm_hour >= 2 && now->tm_hour < 3);
}

void backupRecordings()
{
    int status = system(store_all_recordings.c_str());
    if (status != 0)
    {
        sendMailRecordingFail(backup_error);
    }
}

void backupAndRestart()
{
    backupRecordings();
    restartFtp();
    checkStorageUsage();
}
void mainLoop()
{
    //sleepSec(2);
    while(1)
    {
        if (dirContainsRecording() == false)
        {
            sendMailRecordingFail(no_recordings);
        }
        int currentSize = 0;
        while(currentTimeBetween2and3AM() == false)
        {
            sleepHour();
            int sum_of_sizes = getAllRecordingSizes();
            if (currentSize >= sum_of_sizes)
            {
                sendMailRecordingFail(no_record_increment);
            }
            else 
            {
                currentSize += sum_of_sizes;
            }
        }
        backupAndRestart();
        sleepSec(60);
        //checkDir(); 
        //listFiles();
        //restartFtp();
    }
}
void sig_handler(int signo)
{
      if (signo == SIGINT)
              printf("received SIGINT\n");
      exit(1);
}

int main()
{
    signal(SIGINT, sig_handler);
    sleepSec(60);
    sendMailRecordingFail(greeting);
    mainLoop();



	//restartFtp();
	//printf("%d dircont\n", dirContainsRecording());
	//printf("%d\n", currentTimeBetween2and3AM());
	//sendMailRecordingFail(no_record_increment);
	//checkStorageUsage();
	//int val = getAllRecordingSizes();
    return 0;
}
