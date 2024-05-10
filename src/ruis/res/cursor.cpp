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

#include "cursor.hpp"

#include "../context.hpp"
#include "../util/util.hpp"

using namespace ruis::res;

ruis::res::cursor::cursor(
	utki::shared_ref<ruis::context> c,
	utki::shared_ref<ruis::res::image> image,
	const vector2& hotspot
) :
	resource(std::move(c)),
	image_v(std::move(image)),
	hotspot_v(hotspot)
{}

utki::shared_ref<cursor> cursor::load(
	utki::shared_ref<ruis::context> ctx,
	const tml::forest& desc,
	const papki::file& fi
)
{
	vector2 hotspot;
	bool hotspot_set = false;

	const tml::leaf* image_res_id = nullptr;
	for (auto& p : desc) {
		if (p.value == "image") {
			image_res_id = &get_property_value(p);
		} else if (p.value == "hotspot") {
			hotspot = parse_vec2(p.children);
			hotspot_set = true;
		}
	}

	if (!image_res_id) {
		throw std::logic_error("cursor::load(): resource description does not contain 'image' property");
	}

	if (!hotspot_set) {
		throw std::logic_error("cursor::load(): resource description does not contain 'hotspot' property");
	}

	auto cursor_im = ctx.get().loader.load<res::image>(image_res_id->string);
	return utki::make_shared<cursor>( //
		std::move(ctx),
		std::move(cursor_im),
		hotspot
	);
}
