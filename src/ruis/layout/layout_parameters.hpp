/*
ruis - GUI framework

Copyright (C) 2012-2024  Ivan Gagis <igagis@gmail.com>

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

#include <tml/tree.hpp>

#include "../config.hpp"
#include "../util/align.hpp"
#include "../util/dimension.hpp"
#include "../util/length.hpp"
#include "../util/units.hpp"

namespace ruis {

/**
 * @brief Layout parameters.
 */
struct layout_parameters {
	constexpr const static dimension min = dim::min;
	constexpr const static dimension max = dim::max;
	constexpr const static dimension fill = dim::fill;

	/**
	 * @brief desired dimensions.
	 */
	r4::vector2<dimension> dims;

	/**
	 * @brief Weight of the widget.
	 * Weight defines how much space widget occupies in addition to its minimal or explicitly set size.
	 * Default weight is 0, which means that the widget will not occupy extra space.
	 * Value less than 0 is invalid, default weight will be assumed.
	 */
	real weight = -1;

	/**
	 * @brief Alignment of the widget within its parent.
	 * Horizontal and vertical alignment.
	 */
	r4::vector2<ruis::align> align = {ruis::align::undefined, ruis::align::undefined};

	static layout_parameters make(const tml::forest& desc, const ruis::units& units);
};

using lp = layout_parameters;

}; // namespace ruis
