#include <string>
#include <string.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/file.h>
#include <sys/stat.h>

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
    using namespace rapidjson;
 
    Document *document = new Document();
    {
        char readBuffer[65536];
        FILE* fp = fopen(filepath->c_str(), "rb");
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        document->ParseStream(is);
        fclose(fp);
    }
    this->document = document;
    delete filepath;
}

TaskInfo::~TaskInfo()
{
    delete this->document;
}

TaskStatus* TaskInfo::get_status()
{
    int process_id = (*this->document)["pid"].GetInt();
    int start_time = (*this->document)["start_time"].GetInt();
    
    struct stat file_details;
    bool io_error;
    {
        char process_file_path[20];
        sprintf(process_file_path, "/proc/%d", process_id);
        io_error = lstat(process_file_path, &file_details);
    }

    TaskStatus *task_status = (TaskStatus*) malloc(sizeof(task_status));
    task_status->code = STOPPED;

    if (!io_error)
    {
        if (start_time == file_details.st_ctim.tv_nsec)
        {
            task_status->code = RUNNING;
        }
    }

    return task_status;
}

int TaskInfo::get_pid()
{
    int process_id = (*this->document)["pid"].GetInt();
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
        Value start_time_value;
        {
            struct stat file_details;
            {
                char process_file_path[20];
                sprintf(process_file_path, "/proc/%d", process_id);
                lstat(process_file_path, &file_details);
            }
            start_time_value.SetInt(file_details.st_ctim.tv_nsec);
        }
        d.AddMember("start_time", start_time_value, d.GetAllocator());
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