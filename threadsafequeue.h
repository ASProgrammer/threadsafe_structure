#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <memory>
#include <mutex>
#include <condition_variable>

namespace threadsafe {

template <typename T>
class ThreadSafeQueue {
    struct Node {
        std::shared_ptr<T> m_data;
        std::unique_ptr<Node> m_next;

        Node();
        explicit Node(T& value);
    };

public:
    ThreadSafeQueue();
    ThreadSafeQueue(const std::initializer_list<T>& init);

    void push(const T& value);

    std::shared_ptr<T> try_pop();
    bool try_pop(T& value);

    std::shared_ptr<T> wait_and_pop();
    void wait_and_pop(T& value);

    bool empty() const;

private:
    mutable std::mutex m_headMutex;
    std::unique_ptr<Node> m_head;

    mutable std::mutex m_tailMutex;
    Node* m_tail;
};

}

#endif // THREADSAFEQUEUE_H
