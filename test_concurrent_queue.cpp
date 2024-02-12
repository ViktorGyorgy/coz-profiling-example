#include <iostream>
#include <fstream>
#include <set>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include "./concurrent_queue.cpp"

const size_t QUEUE_SIZE = 1;
const int THREAD_NR = 100;
const int NUMBER_RANGE = 10000;


void consume_queue(concurrent_queue<int, QUEUE_SIZE> *my_queue, std::mutex* write_lock, std::set<int>* set_of_numbers){
    int x = 0;
    while((x = my_queue->pop()) != -1){
        write_lock->lock();
        set_of_numbers->insert(x);
        // std::cout << x << std::endl;
        write_lock->unlock();
    }
}

int main(){
    concurrent_queue<int, QUEUE_SIZE> my_queue;

    //create threads;
    std::set<int> set_of_numbers;
    std::mutex write_lock;
    std::vector<std::thread> threads;
    for(int i = 0; i < THREAD_NR; i++){
        threads.push_back(std::thread{consume_queue, &my_queue, &write_lock, &set_of_numbers});
    }

    for(int i = 0; i < NUMBER_RANGE; i++){
        my_queue.push(i);
    }

    //POISON PILL
    for(int i = 0; i < THREAD_NR; i++){
        my_queue.push(-1);
    }

    for(int i = 0; i < THREAD_NR; i++){
        threads[i].join();
    }
    
    std::cout << set_of_numbers.size();
    return 0;
}