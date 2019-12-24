#include <string>
#include <string.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/file.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include "TaskInfo.h"

const int PERMISSIONS = 
    S_IRUSR | S_IRGRP | S_IROTH |
    S_IWUSR | S_IRGRP | S_IWOTH
;

TaskInfo::TaskInfo(std::string *filepath)
{
    this->filepath = filepath;
}

TaskInfo::~TaskInfo()
{
    delete this->filepath;
}

TaskStatus* TaskInfo::get_status()
{
    return NULL;
}

int TaskInfo::get_pid()
{
    using namespace rapidjson;
 
    Document d;
    {
        char readBuffer[65536];
        FILE* fp = fopen(this->filepath->c_str(), "rb");
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        d.ParseStream(is);
        fclose(fp);
    }

    int process_id = d["pid"].GetInt();

    return process_id;
}


void create_task_file(char const* home, int task_id, int process_id)
{
    const char* contents;
    rapidjson::StringBuffer buffer;
    {
        using namespace rapidjson;    
        Document d;
        d.SetObject();
        Value pid_value;
        pid_value.SetInt(process_id);
        d.AddMember("pid", pid_value, d.GetAllocator());
        Writer<StringBuffer> writer(buffer);
        d.Accept(writer);    
        contents = buffer.GetString();
    }

    int info_file;
    {
        char info_file_name[10];
        sprintf(info_file_name, "/tasks/%d", task_id);
        std::string info_file_path = std::string(home);
        info_file_path.append(info_file_name);
        info_file = open(info_file_path.c_str(), O_RDWR | O_CREAT, PERMISSIONS);
    }

    write(info_file, contents, strlen(contents));
    close(info_file);
}