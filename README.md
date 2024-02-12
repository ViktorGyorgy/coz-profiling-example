# Multithreaded log parser
A project which shows the 10 most requested static image of a REST API. The original log file has around 3GB, downsized it to 100_000 lines for faster runtime and to upload it to github.

# Multithreaded and singlethreaded
Written both solutions, to compare results (if multithreaded gives right answer) and runtimes.  The multithreaded version is around 4 times faster.
<img src="https://github.com/ViktorGyorgy/multithread-log-parser/blob/main/screenshots/times_to_run.png"> <br>


# Concurrent queue
Realized using mutex and semaphores. The pop is thread safe. It is assumed that the main thread usees the push function only.  
Tested for queue size of 1, 100, 5000 with 10 threads.

# Compile executables
Singlethreaded: ```g++ -Wall .\file_parser_sequential.cpp -o parser_single_thread.exe -std=c++20```<br>
Multithreaded: ```g++ -Wall .\file_parser.cpp -o parser_multi_thread.exe -std=c++20```

# Compare runtime
Singlethreaded: ```Measure-Command { .\parser_single_thread.exe } | FINDSTR 'TotalSeconds .*'```<br>
Multithreaded: ```Measure-Command { .\parser_multi_thread.exe } | FINDSTR 'TotalSeconds .*'```

