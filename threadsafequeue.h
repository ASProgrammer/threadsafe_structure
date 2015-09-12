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
    Node* get_tail();
    std::unique_ptr<Node> pop_head();

    std::unique_ptr<Node> try_pop_head();
    std::unique_ptr<Node> try_pop_head(T& value);

private:
    mutable std::mutex m_head_mutex;
    std::unique_ptr<Node> m_head;

    mutable std::mutex m_tail_mutex;
    Node* m_tail;
};

template <typename T>
ThreadSafeQueue<T>::Node::Node():
    m_data { nullptr },
    m_next { nullptr }
{ }

template <typename T>
ThreadSafeQueue<T>::Node::Node(T& value):
    m_data { std::make_shared<T>(value) },
    m_next { nullptr }
{ }

template <typename T>
void ThreadSafeQueue<T>::push(const T& value)
{
    auto new_data = std::make_shared<T>(value);
    auto new_tail = std::unique_ptr<Node> { new Node { } };
    auto p = new_tail.get();

    std::lock_guard<std::mutex> locker { m_tail_mutex };
    m_tail->m_next = std::move(new_tail);
    m_tail->m_data = new_data;
    m_tail = p;
}

template <typename T>
typename ThreadSafeQueue<T>::Node*
ThreadSafeQueue<T>::get_tail()
{
    std::lock_guard<std::mutex> locker { m_tail_mutex };
    return m_tail;
}

template <typename T>
std::unique_ptr<typename ThreadSafeQueue<T>::Node>
ThreadSafeQueue<T>::pop_head()
{
    auto old_head = std::move(m_head);
    m_head = std::move(old_head->m_next);
    return old_head;
}

template <typename T>
std::unique_ptr<typename ThreadSafeQueue<T>::Node>
ThreadSafeQueue<T>::try_pop_head()
{
    std::lock_guard<std::mutex> locker { m_head_mutex };
    if (m_head.get() == get_tail())
        return nullptr;
    return pop_head();
}

template <typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::try_pop()
{
    auto old_head = try_pop_head();
    return old_head ? old_head->m_data : nullptr;
}

template <typename T>
std::unique_ptr<typename ThreadSafeQueue<T>::Node>
ThreadSafeQueue<T>::try_pop_head(T& value)
{
    std::lock_guard<std::mutex> locker { m_head_mutex };
    if (m_head.get() == get_tail())
        return nullptr;

    value = std::move(*m_head->m_data);
    return pop_head();
}

template <typename T>
bool ThreadSafeQueue<T>::try_pop(T& value)
{
    auto old_head = try_pop_head(value);
    return static_cast<bool>(old_head);
}

}

#endif // THREADSAFEQUEUE_H
