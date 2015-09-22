#ifndef STACK_H
#define STACK_H

#include <boost/noncopyable.hpp>
#include <memory>
#include <mutex>

template <typename T>
class ThreadSafeStack: public boost::noncopyable {
    struct Node {
        std::shared_ptr<T> m_data;
        std::unique_ptr<Node> m_next;

        explicit Node(const T& value);
    };

public:
    Stack();
    virtual ~Stack();

    void push(T& value);

    std::shared_ptr<T> try_pop();
    bool try_pop(T& value);

    bool empty() const;

private:
    std::mutex m_mutex;
    std::unique_ptr<Node> m_head;
};

template <typename T>
Stack<T>::Node::Node(const T& value):
    m_head { std::make_shared<T>(value) },
    m_next { nullptr }
{ }

template <typename T>
Stack<T>::Stack():
    m_head { nullptr }
{ }

template <typename T>
Stack<T>::~Stack()
{ }

template <typename T>
void Stack<T>::push(T &value)
{
    auto new_head = std::make_unique<Node>(value);

    std::lock_guard<std::mutex> locker { m_mutex };
    new_head->m_next = std::move(m_head);
    m_head = std::move(new_head);
}

template <typename T>
bool Stack<T>::empty() const
{
    std::lock_guard<std::mutex> locker { m_mutex };
    return static_cast<bool>(m_head);
}

template <typename T>
std::shared_ptr<T> Stack<T>::try_pop()
{
    std::lock_guard<std::mutex> locker { m_mutex };
    if (!m_head)
        return nullptr;

    auto old_head = std::move(m_head);
    m_head = old_head->m_next;

    return old_head->m_data;
}

#endif // STACK_H
