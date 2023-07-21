/*
morda - GUI framework

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

#include "vertex_array.hpp"

#include <utki/debug.hpp>

using namespace morda;

vertex_array::vertex_array(
	decltype(buffers)&& buffers, // TODO: pass by value?
	// NOLINTNEXTLINE(modernize-pass-by-value)
	const utki::shared_ref<const morda::index_buffer>& indices,
	mode rendering_mode
) :
	buffers(std::move(buffers)),
	indices(indices),
	rendering_mode(rendering_mode)
{
	if (this->buffers.empty()) {
		throw std::invalid_argument("no vertex buffers passed to vertex array");
	}
	auto s = this->buffers.front().get().size;
	for (auto& b : this->buffers) {
		if (b.get().size != s) {
			throw std::invalid_argument(
				"vertex buffers of different size passed in to vertex array, should all be of the same size"
			);
		}
	}
}
