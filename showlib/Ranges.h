#pragma once

#include <algorithm>

// C++20 has the ranges: namespace, but we're compiling under C++17.
// I got this idea from Jonathan's talk, with examples, and he got it
// from somewhere else. I'm implementing the ones I find interesting.
// They're pretty trivial.
//
namespace Ranges {
    template<typename Range, typename Function>
    Function for_each(Range& range, Function f)
    {
        return std::for_each(begin(range), end(range), f);
    }

    template<typename Range, typename Function>
    auto for_each_n(Range& range, size_t n, Function f)
    {
        return std::for_each_n(begin(range), n, f);
    }

    template<typename Range, typename Function>
    auto is_heap(Range& range, Function f)
    {
        return std::is_heap(begin(range), end(range), f);
    }

    template<typename Range, typename Function>
    auto is_heap_until(Range& range, Function f)
    {
        return std::is_heap_until(begin(range), end(range), f);
    }

    template<typename Range, typename Function>
    auto make_heap(Range& range, Function f)
    {
        return std::make_heap(begin(range), end(range), f);
    }

    template<typename Range, typename Function>
    auto push_heap(Range& range, Function f)
    {
        return std::push_heap(begin(range), end(range), f);
    }

    template<typename Range, typename Function>
    auto pop_heap(Range& range, Function f)
    {
        return std::pop_heap(begin(range), end(range), f);
    }

    template<typename Range, typename Function>
    auto sort_heap(Range& range, Function f)
    {
        return std::sort_heap(begin(range), end(range), f);
    }

    template<typename Range, typename ObjectType>
    auto find(Range &range, const ObjectType &value)
    {
        return std::find(begin(range), end(range), value);
    }

    template<typename Range, typename Function>
    auto find_if(Range &range, const Function &f)
    {
        return std::find_if(begin(range), end(range), f);
    }

    template<typename Range, typename Function>
    auto find_if_not(Range &range, const Function &f)
    {
        return std::find_if_not(begin(range), end(range), f);
    }

    template<typename Range, typename ObjectType>
    auto remove(Range &range, const ObjectType &value)
    {
        return std::remove(begin(range), end(range), value);
    }

    template<typename Range, typename Function>
    auto remove_if(Range &range, const Function &f)
    {
        return std::remove_if(begin(range), end(range), f);
    }

    template<typename Range>
    auto unique(Range &range)
    {
        return std::unique(begin(range), end(range));
    }

    template<typename Range, typename Function>
    auto unique_if(Range &range, const Function &f)
    {
        return std::unique(begin(range), end(range), f);
    }

    // std::erase_if is C++20, and I'm building under
    // C++17, so we'll do it a little differently.
    template<typename Range, typename Function>
    auto erase_if(Range &range, const Function &f)
    {
        return range.erase(std::remove(begin(range), end(range), f), end(range));
    }
}
