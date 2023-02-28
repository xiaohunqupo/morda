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

#include "size_container.hpp"

using namespace morda;

size_container::size_container(const utki::shared_ref<morda::context>& c, const treeml::forest& desc) :
		widget(c, desc),
		container(this->context, desc)
{}

void size_container::lay_out(){
	for(auto& w : this->children()){
		if(w.get().is_layout_dirty()){
			auto d = this->dims_for_widget(w.get(), w.get().get_layout_params_const());
			w.get().resize(d);
		}
	}
}
