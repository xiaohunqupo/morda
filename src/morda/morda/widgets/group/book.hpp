/*
morda GUI framework
Copyright (C) 2021  Ivan Gagis <igagis@gmail.com>

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

#include "pile.hpp"

namespace morda{

class book;

class page : public virtual widget{
	friend class book;
protected:
	page(std::shared_ptr<morda::context> c, const treeml::forest& desc);
	
	page(const page&) = delete;
	page& operator=(const page&) = delete;

	book* parent_book = nullptr;
public:
	book& get_parent_book();
	const book& get_parent_book()const;

	void go_to();

	virtual void on_show(){}
	virtual void on_hide()noexcept{}
	
	void tear_out()noexcept;
private:

};

class book :
		public virtual widget,
		private pile
{
	friend class page;
	
	size_t active_page_index = std::numeric_limits<size_t>::max(); // invalid index

	std::vector<std::shared_ptr<page>> pages;
public:
	book(std::shared_ptr<morda::context> c, const treeml::forest& desc);
	
	book(const book&) = delete;
	book& operator=(const book&) = delete;
	
	void push(std::shared_ptr<page> page);
	
	size_t size()const{
		return this->pages.size();
	}

	size_t get_active_page()const{
		return this->active_page_index;
	}

	void go_to(size_t page_number);
	void go_to(const page& p);

	~book()noexcept;

	// page is either pushed or teared out
	std::function<void(book&, const page&)> pages_change_handler;
private:
	void tear_out(page& page)noexcept;

	void notify_pages_change(const page& p);
};

}
