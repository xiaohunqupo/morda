/*
ruis - GUI framework

Copyright (C) 2012-2023  Ivan Gagis <igagis@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ================ LICENSE END ================ */

#pragma once

#include <r4/rectangle.hpp>
#include <r4/vector.hpp>
#include <treeml/tree.hpp>
#include <utki/config.hpp>

#include "../config.hpp"

#include "units.hpp"

namespace ruis {

ruis::vector2 parse_vec2(treeml::forest::const_iterator begin, treeml::forest::const_iterator end);

/**
 * @brief Parse 2 values from treeml as vector2.
 * @param desc - forest of at least two trees holding vector2 values.
 *                If there are less than 2 trees in the forest then the rest of
 *                vector components will be filled with latest parsed value.
 * @return parsed vector2.
 */
inline ruis::vector2 parse_vec2(const treeml::forest& desc)
{
	return parse_vec2(desc.begin(), desc.end());
}

/**
 * @brief Parse chain of 4 treeml nodes as rectangle.
 * @param desc - chain of at least four nodes holding rectangle values.
 *               If there are less than 4 nodes in the chain then the rest of
 *               rectangle components will be filled with latest parsed value.
 *               If zero pointer is passed the resulting rectangle will be filled with zeros.
 * @return parsed rectangle.
 */
ruis::rectangle parse_rect(const treeml::forest& desc);

/**
 * @brief Parse chain of 4 treeml nodes as sides.
 * @param desc - chain of at least four nodes holding sides values.
 *               If there are less than 4 nodes in the chain then the rest of
 *               sides<real> components will be filled with latest parsed value.
 *               If zero pointer is passed the resulting sides<real> will be filled with zeros.
 * @return parsed sides<real>.
 */
ruis::sides<real> parse_sides(const treeml::forest& desc);

/**
 * @brief Parse dimension value.
 * Parses value of dimension property from treeml leaf.
 * In case the value is given in millimeters or points it will do the conversion.
 * @param l - treeml leaf holding the value.
 * @param units - information about units. Can be obtained from context.
 * @return Parsed value in pixels.
 */
real parse_dimension_value(const treeml::leaf& l, const ruis::units& units);

/**
 * @brief Parse layout dimension value.
 * Parses value of dimension value of layout parameters from treeml.
 * @param l - treeml leaf holding the value.
 * @param units - information about units. Can be obtained from context.
 * @return Parsed value.
 */
real parse_layout_dimension_value(const treeml::leaf& l, const ruis::units& units);

bool is_property(const treeml::tree& t);

bool is_leaf_property(const treeml::leaf& l);

bool is_leaf_child(const treeml::leaf& l);

bool is_variable(const tml::tree& t);

r4::vector4<float> color_to_vec4f(uint32_t color);

inline const treeml::leaf& get_property_value(const treeml::tree& p)
{
	if (p.children.size() != 1) {
		throw std::invalid_argument("get_property_value(): property has no value");
	}
	return p.children.front().value;
}

} // namespace ruis