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
#ifndef _HADOKEN_GEOMETRY_ALGORITHMS_HPP
#define _HADOKEN_GEOMETRY_ALGORITHMS_HPP

#include <cmath>

#include <boost/geometry/geometry.hpp>
#include <hadoken/geometry/objects/objects.hpp>


namespace hadoken{

namespace geometry{

namespace cartesian {

/// coordinate mapper
template<typename CoordType>
struct coordinate_type{

};

template<typename CoordType>
struct coordinate_type<point3<CoordType>>{
        typedef CoordType type;
};


/// accessor x coordinate
template<typename Point>
typename coordinate_type<Point>::type get_x(const Point & p){
    return bg::get<0>(p);
}

/// accessor y coordinate
template<typename Point>
typename coordinate_type<Point>::type get_y(const Point & p){
    return bg::get<1>(p);
}

/// accessor z coordinate
template<typename Point>
typename coordinate_type<Point>::type get_z(const Point & p){
    return bg::get<2>(p);
}

//// make point
template<typename CoordType>
point3<CoordType> make_point(const std::array<CoordType, 3> & coord ){
    return point3<CoordType>(coord[0], coord[1], coord[2]);
}


template<typename Vector>
Vector cross_product(const Vector & v1, const Vector & v2){
    using coord_type = typename coordinate_type<Vector>::type;

    const coord_type x = get_y(v1) * get_z(v2) - get_z(v1) * get_y(v2);
    const coord_type y = get_z(v1) * get_x(v2) - get_x(v1) * get_z(v2);
    const coord_type z = get_x(v1) * get_y(v2) - get_y(v1) * get_x(v2);
    return Vector(x,y,z);
}


template<typename Vector>
typename coordinate_type<Vector>::type norm(const Vector & v1){
    using coord_type = typename coordinate_type<Vector>::type;

    const coord_type x = get_x(v1);
    const coord_type y = get_y(v1);
    const coord_type z = get_z(v1);

    return std::sqrt(x*x + y*y +z*z);
}

template<typename Vector>
Vector normalize(const Vector & v1){
    using coord_type = typename coordinate_type<Vector>::type;
    Vector res(v1);
    const coord_type norm_value = norm<Vector>(v1);

    if(hadoken::math::close_to_abs<coord_type>(norm_value, 0.0)){
        throw std::logic_error("try to normalize a vector of norm 0");
    }
    res /= norm_value;
    return res;
}


} // cartesian


} // geometry

} // hadoken



#endif // ALGORITHMS_HPP

