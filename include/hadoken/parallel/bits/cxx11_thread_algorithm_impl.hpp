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
#ifndef _HADOKEN_CXX11_THREAD_ALGORITHM_BITS_HPP_
#define _HADOKEN_CXX11_THREAD_ALGORITHM_BITS_HPP_

#include <type_traits>
#include <future>
#include <thread>
#include <stdexcept>
#include <atomic>

#include <hadoken/containers/small_vector.hpp>
#include <hadoken/parallel/algorithm.hpp>
#include <hadoken/utility/range.hpp>
#include <hadoken/executor/system_executor.hpp>

namespace hadoken{


namespace parallel{


class sequential_execution_policy;
class parallel_execution_policy;
class parallel_vector_execution_policy;



namespace detail{

using namespace hadoken::containers;

std::size_t get_parallel_task(){
    return std::thread::hardware_concurrency();
}

/// for_each algorithm
template<typename Iterator, typename Function>
inline Function _simple_cxx11_parallel(Iterator begin_it, Iterator end_it, Function fun){
    const std::size_t n_task = get_parallel_task();

    std::atomic<std::size_t> completed_task(0);

    range<Iterator> global_range(begin_it, end_it);

    system_executor sexec;

    std::mutex mut;
    std::condition_variable cond;

    // start task for tasks 1-N on other cores
    for(std::size_t i = 1; i < n_task; ++i){

         sexec.execute([i, &cond, &completed_task, &mut, &global_range, &n_task, &fun](){
            auto my_range = hadoken::take_splice(global_range, i, n_task);
            std::for_each(my_range.begin(), my_range.end(), fun);
            cond.notify_one();
            completed_task++;
        });
    }

    // execute the task 0 locally
    auto my_range = hadoken::take_splice(global_range, 0, n_task);
    std::for_each(my_range.begin(), my_range.end(), fun);

    // wait for the folks
    while(completed_task.load() < (n_task -1)){
        std::unique_lock<std::mutex> _l(mut);
        cond.wait_for(_l, std::chrono::microseconds(10));
    }


    return fun;
}



} // detail


/// for_each algorithm
template<typename ExecPolicy, typename Iterator, typename Function>
inline Function for_each(ExecPolicy && policy, Iterator begin_it, Iterator end_it, Function fun){
    (void) policy;
    if(std::is_same<ExecPolicy, parallel_execution_policy>::value
            || std::is_same<ExecPolicy, parallel_vector_execution_policy>::value ){
        return detail::_simple_cxx11_parallel(begin_it, end_it, fun);
    }

   return std::for_each(begin_it, end_it, fun);
}


} // concurrent



} // hadoken


#endif
