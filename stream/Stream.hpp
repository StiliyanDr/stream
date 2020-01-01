#include <assert.h>
#include "Utility.h"

template <class InputIterator>
Stream<InputIterator>::Stream(InputIterator begin,
                              InputIterator end) :
    begin(begin),
    end(end)
{
}

template <class InputIterator>
Stream<InputIterator>::Stream(Stream&& source) :
    begin(source.begin),
    end(source.end),
    cached_item(std::move(source.cached_item))
{
    source.empty();
}

template <class InputIterator>
inline void Stream<InputIterator>::empty()
{
    begin = end;
    cached_item.reset();
}

template <class InputIterator>
Stream<InputIterator>&
Stream<InputIterator>::operator=(Stream&& rhs)
{
    if (this != &rhs)
    {
        swap_contents_with(rhs);
        rhs.empty();
    }

    return *this;
}

template <class InputIterator>
void Stream<InputIterator>::swap_contents_with(Stream& s)
{
    using std::swap;

    swap(this->cached_item, s.cached_item);
    swap(this->begin, s.begin);
    swap(this->end, s.end);
}

template <class InputIterator>
template <class UnaryPredicate>
inline auto Stream<InputIterator>::take_unless(UnaryPredicate p)
-> std::optional<Item>
{
    return take_if(utils::negation_of(p));
}

template <class InputIterator>
template <class UnaryPredicate>
auto Stream<InputIterator>::take_if(UnaryPredicate p)
-> std::optional<Item>
{
    auto result = take_one();

    if (result.has_value() &&
        !p(*result))
    {
        cached_item = utils::move_assign_if_noexcept(result);
        result.reset();
    }

    return result;
}

template <class InputIterator>
auto Stream<InputIterator>::take_one()
-> std::optional<Item>
{
    auto result = std::optional<Item>{};

    if (cached_item.has_value())
    {
        result = extract_cached_item();
    }
    else if (begin != end)
    {
        result = *begin;
        ++begin;
    }

    return result;
}

template <class InputIterator>
auto Stream<InputIterator>::extract_cached_item()
-> Item
{
    assert(cached_item.has_value());

    auto result = std::move_if_noexcept(*cached_item);
    cached_item.reset();

    return result;
}

template <class InputIterator>
template <class UnaryPredicate>
inline void Stream<InputIterator>::drop_unless(UnaryPredicate p)
{
    drop_if(utils::negation_of(p));
}

template <class InputIterator>
template <class UnaryPredicate>
inline void Stream<InputIterator>::drop_if(UnaryPredicate p)
{
    take_if(p);
}

template <class InputIterator>
void Stream<InputIterator>::drop_one()
{
    if (cached_item.has_value())
    {
        cached_item.reset();
    }
    else if (begin != end)
    {
        ++begin;
    }
}

template <class InputIterator>
inline bool Stream<InputIterator>::is_empty() const
{
    return !cached_item.has_value() &&
           begin == end;
}
