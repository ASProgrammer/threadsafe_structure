#ifndef STACK_H
#define STACK_H

#include <boost/noncopyable.hpp>
#include <memory>
#include <mutex>

template <typename T>
class Stack: public boost::noncopyable {
    struct Node {
        std::shared_ptr<T> m_data;
        std::unique_ptr<Node> m_next;
    };

public:
    Stack();
    virtual ~Stack();

    void push(T& value);

    std::shared_ptr<T> pop();
    bool pop(T& value);

    bool empty() const;

private:
    std::mutex m_mutex;
    std::unique_ptr<Node> m_head;
};

#endif // STACK_H
