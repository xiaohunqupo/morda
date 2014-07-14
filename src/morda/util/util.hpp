/* The MIT License:

Copyright (c) 2012-2014 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://morda.googlecode.com

/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include "Vector2.hpp"

#include <stob/dom.hpp>
#include <ting/math.hpp>


namespace morda{



/**
 * @brief Parse chain of 2 STOB nodes as Vec2f.
 * @param chain - chain of at least two nodes holding Vec2f values.
 *                If there are less than 2 nodes in the chain then the rest of
 *                vector components will be filled with latest parsed value.
 *                If zero pointer is passed the resulting vector will be filled with zeros.
 * @return parsed Vec2f.
 */
morda::Vec2f Vec2fFromSTOB(const stob::Node* chain);



/**
 * @brief Parse dimension Vec2f from STOB.
 * Same as Vec2fFromSTOB but using DimValue() to parse values.
 * @param chain - chain of at least two nodes holding Vec2f values.
 *                If there are less than 2 nodes in the chain then the rest of
 *                vector components will be filled with latest parsed value.
 *                If zero pointer is passed the resulting vector will be filled with zeroes.
 * @return parsed Vec2f.
 */
morda::Vec2f DimVec2f(const stob::Node* chain);



/**
 * @brief Round each component of Vec2f.
 * Call ting::math::Round() for each component of given Vec2f.
 * @param v - Vec2f to round.
 * @return Rounded Vec2f.
 */
inline morda::Vec2f RoundVec(const Vec2f& v){
	return Vec2f(ting::math::Round(v.x), ting::math::Round(v.y));
}



/**
 * @brief Parse dimension value.
 * Parses value of 'pos' or 'dim' property from STOB.
 * In case the value is given in millimeters it will do the conversion.
 * @param n - stob node holding the value.
 * @return Parsed value in pixels.
 */
float DimValue(const stob::Node& n);



/**
 * @brief Resolve includes in STOB document.
 * @param fi - file interface set to the original STOB document. Because resolving include paths is done relatively to original STOB document path.
 * @param root - root node of the original STOB document.
 * @return Pointer to the last child node of the script.
 */
stob::Node* ResolveIncludes(ting::fs::File& fi, stob::Node& root);



}//~namespace
