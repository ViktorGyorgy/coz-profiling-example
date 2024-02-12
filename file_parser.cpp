#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <regex>
#include <string>
#include <thread>
#include <vector>
#include "./concurrent_queue.cpp"

const int THREAD_NR = 5;
const size_t QUEUE_SIZE = 5000;
const std::string POISON_PILL = "THREADS SHOULD BE ENDED!";
const int LINES_TO_BE_PROCCESSED = 100000;

bool cmp_reverse(std::pair<std::string, int>& a, std::pair<std::string, int>& b) 
{ 
    return a.second > b.second; 
} 


std::vector<std::pair<std::string, int>> ten_most_popular_static_images
(std::map<std::string, int>& incidence_map)
{
    std::vector<std::pair<std::string, int>> sorted_incidence;
    for(auto& p : incidence_map)
    {
        sorted_incidence.push_back(p);
    }   

    std::sort(sorted_incidence.begin(), sorted_incidence.end(), cmp_reverse);
    sorted_incidence.resize(10);
    return sorted_incidence;
} 

void log_consumer(concurrent_queue<std::string, QUEUE_SIZE>* my_queue, 
std::mutex* write_mutex, std::map<std::string, int>* incidence_map)
{
    std::string s;
    const std::regex image_path_regex("GET (/static/images/[^\\s]+)");
    while(POISON_PILL.compare((s = my_queue->pop()))){
        std::regex_iterator image_path_begin = std::sregex_iterator(s.begin(), s.end(), image_path_regex);
        std::regex_iterator image_path_end = std::sregex_iterator();

        for(std::regex_iterator it = image_path_begin; it != image_path_end; it++)
        {
            std::string path_to_file = it->str(1);
            
            write_mutex->lock();
            if(!incidence_map->contains(path_to_file))
            {
                (*incidence_map)[path_to_file] = 0;
            }
            (*incidence_map)[path_to_file]++;
            write_mutex->unlock();
        }
    }
}


int main()
{
    std::ifstream ifs("access.log", std::ifstream::in);
    std::string line;

    std::map<std::string, int> incidence_map;
    concurrent_queue<std::string, QUEUE_SIZE> my_queue;
    std::mutex write_mutex;
    std::vector<std::thread> threads;

    for(int i = 0; i < THREAD_NR; i++)
    {
        threads.push_back(std::thread{log_consumer, &my_queue, &write_mutex, &incidence_map});
    }

    while(std::getline(ifs, line))
    {
        my_queue.push(line);
    };
    ifs.close();

    for(int i = 0; i < THREAD_NR; i++)
    {
        my_queue.push(POISON_PILL);
    }

    for(int i = 0; i < THREAD_NR; i++)
    {
        threads[i].join();
    }


    std::vector<std::pair<std::string, int>> incidence_sorted = ten_most_popular_static_images(incidence_map);
    std::ofstream ofs("results.txt");
    for(auto it= incidence_sorted.begin(); it != incidence_sorted.end(); it++)
    {
        ofs << it->first << " " << it->second << std::endl;
    }
    ofs.close();
    return 0;
}