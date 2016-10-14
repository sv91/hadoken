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




#include <boost/test/floating_point_comparison.hpp>


#include <boost/random.hpp>
#include <boost/chrono.hpp>

#include <hadoken/containers/small_vector.hpp>


using namespace boost::chrono;

typedef  system_clock::time_point tp;
typedef  system_clock cl;

constexpr std::size_t static_vec_size = 64;
const std::size_t n_elems= 64;

std::size_t test_std_vector(std::size_t iter){



    std::size_t res =0;

    tp t1, t2;

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        std::vector<std::size_t> v;
        for(std::size_t j = 0; j < n_elems; ++j){
            v.push_back(j);
        }
        res = std::accumulate(v.begin(), v.end(), res);
    }


    t2 = cl::now();

    std::cout << "vector: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}


std::size_t test_small_vector(std::size_t iter){

    using namespace hadoken::containers;

    std::size_t res =0;

    tp t1, t2;

    t1 = cl::now();

    for(std::size_t i =0; i < iter; ++i){
        small_vector<std::size_t, static_vec_size> v;

        for(std::size_t j = 0; j < n_elems; ++j){
            v.push_back(j);
        }
        res = std::accumulate(v.begin(), v.end(), res);
    }


    t2 = cl::now();

    std::cout << "small_vector: " << boost::chrono::duration_cast<milliseconds>(t2 -t1) << std::endl;
    return res;

}




int main(){

    const std::size_t n_exec = 1000000;
    std::size_t junk=0;

    std::cout << "generation of " << n_exec << "*" << n_elems  << " insertions" << std::endl;

    junk += test_small_vector(n_exec);


    junk += test_std_vector(n_exec);

    std::cout << "end junk " << junk << std::endl;

}
