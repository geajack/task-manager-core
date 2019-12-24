# Task Manager Core

This is a task management library for Linux-like operating systems (POSIX systems with a Linux-like `/proc` directory). The library consists of four C functions declared in `tasks.h`: `run`, `stop`, `status` and `logs`. This repository also includes a small CLI app to demonstrate these API calls, in `main.c`.

In practice this library is intended to form the backend of a GUI background task manager app.

## API

The purpose of the API is to allow launching other programs in the background in such a way as to be able to later on report on their status, print their output, or stop them. When the API is used to launch a program, it runs the program in a new process and assigns it a new "Task ID", which is stored on disk along with enough information to allow later stopping the process or obtaining its status.

There is no "host process", that is, no process associated with the task manager itself. Each call to the API requires a path to a directory (the parameter called "home") in which the task manager's state is maintained. The API is designed to work even across reboots and even if the process associated with a task is killed by an unrelated program.

### run
This call accepts a shell command (a program and argumets) and executes it in a new process. It assigns this process a task ID and stores in the given home directory the PID and start time of the process. Storing the start time is necessary to be able to identify the process later on, as PIDs are not unique across reboots.
This call also redirects both the `stdout` and `stderr` of the new process to a file in the given home directory.

### stop
This call accepts a task ID and stops the associated task. It does this by stopping the process with the same PID and start time as the given task, if it exists.

### status
This call accepts a task ID and reports whether or not the given task is currently running, by checking if a process with the same PID and start time is currently running.

### logs
This call accepts a task ID and reports the path to the file in which the `stdout` and `stderr` for the associated process can be found.

## Libraries
This codebase uses the [RapidJSON](https://rapidjson.org/) library, which is [MIT licensed](https://github.com/Tencent/rapidjson/blob/master/license.txt).