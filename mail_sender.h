#ifndef _MAIL_SENDER_H
#define _MAIL_SENDER_H

#include "common.h"

const string mail_scheme = "mail_scheme.txt";
const string mail_file = "mail.txt";

void readFile(vector<string> &lines)
{
    ifstream file(mail_scheme.c_str());
    string line;
    while(getline(file, line))
    {
         lines.push_back(line);
    }
}

void saveMailToFile(vector<string>& lines)
{
    ofstream file(mail_file.c_str());
    for(int i = 0; i < lines.size(); i++)
    {
        file << lines[i] << endl;
    }
    file.close();
    system(send_mail.c_str());
    printf("mail was send\n");
}

void sendMailRecordingFail(string msg)
{
    vector<string> lines;
    readFile(lines);
    lines.push_back(msg);

    saveMailToFile(lines);
}

#endif
