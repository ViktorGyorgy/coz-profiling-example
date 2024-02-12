# Multithreaded log parser
A project which shows the 10 most requested static image of a REST API.  I used regex to get the path of static images from the string. The original log file has around 3GB, in the project I used the first 100000 lines.

# Multithreaded and single threaded
Written both solutions, to compare results (if multithreaded gives right answer) and runtimes. In the multithreaded app, the regex matching and counting of images was done by different threads.  
The multithreaded version is around 4 times faster:
<img src="https://github.com/ViktorGyorgy/multithread-log-parser/blob/main/screenshots/times_to_run.png"> <br>

# Concurrent queue
Realized using mutex and semaphores. The pop is thread safe. It is assumed, that only the main thread uses the push function.  
Tested for queue size of 1, 100, 5000 with 10 threads.

# Log file
The full log file is uploaded into access_all.zip.  
If you want to run the program with n lines from the log file, do:
- unzip access_all.zip into the project folder
- change lines_number in resize_log.py
- run ```python resize_log.py```

# Compile executables
Single threaded: ```g++ -Wall .\file_parser_sequential.cpp -o parser_single_thread.exe -std=c++20```<br>
Multithreaded: ```g++ -Wall .\file_parser.cpp -o parser_multi_thread.exe -std=c++20```

# Compare runtime
Single threaded: ```Measure-Command { .\parser_single_thread.exe } | FINDSTR 'TotalSeconds .*'```<br>
Multithreaded: ```Measure-Command { .\parser_multi_thread.exe } | FINDSTR 'TotalSeconds .*'```

# Results
You can check that the multithreaded version works well by comparing the contents of results.txt and results_sequential.txt. 

