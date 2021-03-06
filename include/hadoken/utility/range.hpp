/**
 * Copyright (c) 2016, Adrien Devresse <adrien.devresse@epfl.ch>
 * 
 * Boost Software License - Version 1.0 
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
* 
*/
#ifndef _HADOKEN_RANGE_HPP_
#define _HADOKEN_RANGE_HPP_


#include <cassert>
#include <algorithm>
#include <vector>
#include <type_traits>

namespace hadoken {


namespace{

template <typename Iterator, typename Extra = void>
inline void iterator_check_range(Iterator first_elem, Iterator end_elem){
    (void) first_elem;
    (void) end_elem;
}

template <typename Iterator, typename std::enable_if<std::is_same<
                                                         typename std::iterator_traits<Iterator>::iterator_category,
                                                         std::random_access_iterator_tag>::value >::type>
inline void iterator_check_range(Iterator first_elem, Iterator end_elem){
    assert(first_elem < end_elem || first_elem == end_elem);
    (void) first_elem;
    (void) end_elem;
}



}


template <typename Iterator>
class range{
public:
    typedef Iterator iterator_type;
    typedef range<Iterator> range_type;

    inline range(const iterator_type & first_elem, const iterator_type & end_elem) : first_(first_elem), end_(end_elem){
        iterator_check_range<iterator_type>(first_elem, end_elem);
    }

    inline const iterator_type & begin() const{
        return first_;
    }

    inline const iterator_type & end() const{
        return end_;
    }

    ///
    /// \brief size
    /// \return number of element in the range
    ///
    inline size_t size() const{
        return std::distance(first_, end_);
    }

    bool operator==(const range_type & other) const{
        return (first_ == other.first_) && (end_ == other.end_);
    }

private:
    iterator_type first_, end_;
};



template<typename Range>
inline std::vector<Range> split_range(const Range & range, std::size_t number_parts){

    assert(number_parts > 0);

    std::vector<Range> ranges;
    const std::size_t min_size = 1;
    const std::size_t size_range = range.size();
    const std::size_t avg_size= size_range/number_parts;
    std::size_t remain_elems = size_range%number_parts;
    ranges.reserve(number_parts);

    typename Range::iterator_type first = range.begin(), last = range.begin();

    do{
        std::size_t segment_size = avg_size + ((remain_elems > 0)?1:0);
        segment_size = std::max<std::size_t>(segment_size, min_size);
        segment_size = std::min<std::size_t>(segment_size, std::distance(first, range.end()));
        std::advance(last, segment_size);
        ranges.push_back( Range(first, last));
        first = last;
        remain_elems = ((remain_elems > 0)?(remain_elems-1):0);
    } while(ranges.size() < number_parts);

    return ranges;
}


template<typename Range>
inline Range take_splice(const Range & range, std::size_t slice_id, std::size_t total_slices){
    assert(my_splice <= total_slices);
    const std::size_t n_elem = range.size();
    const std::size_t elem_per_slice_base = n_elem / total_slices;
    const std::size_t elem_per_slice_modulo = n_elem % total_slices;
    const std::size_t additional_elem = (slice_id < elem_per_slice_modulo) ? 1 : 0;

    typename Range::iterator_type first = range.begin();
    std::advance(first, elem_per_slice_base * slice_id + std::min(elem_per_slice_modulo, slice_id) );

    typename Range::iterator_type last = first;
    std::advance(last, elem_per_slice_base + additional_elem);


    return Range(first, last);
}

}

#endif // RANGE_HPP
