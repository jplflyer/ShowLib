#pragma once
#include <iostream>

// I don't know why the STL people thought list::pop() shouldn't return what it pops,
// but that's just stupid and freakishly annoying. I'm going to define a stack that
// implements push() and pop(), both operating on the front of the stack.
//
// I don't like using std::stack<>, as it's parsimonious on the methods it provides.
// You can't, for instance, iterator over the members. For that reason, I use std::list<>
// instead. I also don't like some of the other behavior. I like pop() to return the
// element popped off, for instance. I imagine they don't do this because they don't
// want to copy the data, but I'm storing shared_ptrs anyway.

#include <list>
#include <memory>

namespace ShowLib {

template <typename T>
class Stack {
public:
    typedef std::shared_ptr<T> Pointer;

    typedef std::_List_iterator<Pointer>			iterator;
    typedef std::_List_const_iterator<Pointer>		const_iterator;
    typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
    typedef std::reverse_iterator<iterator>			reverse_iterator;

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

    /**
     * This pops the front and then returns the new current front.
     */
    Pointer popAndFront() {
        if (stack.empty()) {
            return nullptr;
        }
        stack.pop_front();
        if (stack.empty()) {
            return nullptr;
        }
        return stack.front();
    }

    /**
     * Returns the top of the stack or nullptr if empty.
     */
    Pointer top() {
        if (stack.empty()) {
            return nullptr;
        }
        return stack.front();
    }
    Pointer front() { return top(); }

    /**
     * Return the bottom of the stack, the item pushed first.
     * This probably won't get much usage.
     */
    Pointer bottom() {
        if (stack.empty()) {
            return nullptr;
        }
        return stack.back();
    }

    void foo() {
        for (const Pointer &p: stack) {
            std::cout << "Hubba." << std::endl;
        }
    }

    // All the iterator methods.
    iterator begin() noexcept						{ return stack.begin(); }
    iterator end() noexcept							{ return stack.end(); }
    const_iterator begin() const noexcept			{ return stack.begin(); }
    const_iterator end() const noexcept				{ return stack.end(); }

    reverse_iterator rbegin() noexcept				{ return stack.rbegin(); }
    reverse_iterator rend() noexcept				{ return stack.rend(); }
    const_reverse_iterator rbegin() const noexcept	{ return stack.rbegin(); }
    const_reverse_iterator rend() const noexcept	{ return stack.rend(); }

    const_iterator cbegin() const noexcept			{ return stack.cbegin(); }
    const_iterator cend() const noexcept			{ return stack.cend(); }

    const_reverse_iterator crbegin() const noexcept	{ return stack.crbegin(); }
    const_reverse_iterator crend() const noexcept	{ return stack.crend(); }

private:
    std::list<Pointer> stack;
};

};
