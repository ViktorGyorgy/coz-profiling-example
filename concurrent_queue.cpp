#include <semaphore>
#include <mutex>

//IT IS ASSUMED THAT ONLY ONE THREAD USES THE PUSH
template <typename T, size_t SIZE> 
class concurrent_queue
{
private:
    T* array;
    std::counting_semaphore<SIZE> free_size_semaphore{SIZE};
    std::counting_semaphore<SIZE> readable_semaphore{0};
    std::mutex read_mutex;
    int current_index = 0;
    int insert_index = 0;
public:
    concurrent_queue();
    ~concurrent_queue();
    T pop();
    void push(T entity);
};

template <typename T, size_t SIZE> 
 concurrent_queue<T, SIZE>::concurrent_queue()
{
    this->array = new T[SIZE];
}

template <typename T, size_t SIZE> 
concurrent_queue<T, SIZE>::~concurrent_queue()
{
    delete[] this->array;
}

template <typename T, size_t SIZE>  
T concurrent_queue<T, SIZE>::pop()
{
    read_mutex.lock();

    T entity = this->array[this->current_index];

    this->current_index = (this->current_index + 1) % SIZE;
    free_size_semaphore.release();

    read_mutex.unlock();

    return entity;
}

template <typename T, size_t SIZE> 
void concurrent_queue<T, SIZE>::push(T entitity)
{
    free_size_semaphore.acquire();
    this->array[this->insert_index] = entitity;
    readable_semaphore.release();
    insert_index = (insert_index + 1) % SIZE;
}
