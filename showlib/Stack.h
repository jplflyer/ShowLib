#pragma once

// I don't know why the STL people thought list::pop() shouldn't return what it pops,
// but that's just stupid and freakishly annoying. I'm going to define a stack that
// implements push() and pop(), both operating on the front of the stack.

#include <list>
#include <memory>

namespace ShowLib {

template <typename T>
class Stack {
public:
    typedef std::shared_ptr<T> Pointer;

    bool empty() const { return stack.empty(); }
    size_t size() const { return stack.size(); }

    /**
     * Push an object to the front of the stack. Returns ourself in case
     * you wan to use a builder pattern:
     *
     * 		stack.push(...).push(...);
     */
    Stack<T> & push(Pointer ptr) { stack.push_front(ptr); return *this; }

    /**
     * Pop the front of the stack. Returns nullptr on an empty stack.
     *
     * 		T::Pointer value = stack.pop();
     */
    Pointer pop() {
        if (stack.empty()) {
            return nullptr;
        }
        Pointer ptr = stack.front();
        stack.pop_front();
        return ptr;
    }

private:
    std::list<Pointer> stack;
};

};
