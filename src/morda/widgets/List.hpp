/* The MIT License:

Copyright (c) 2015 Ivan Gagis <igagis@gmail.com>

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

#include "Widget.hpp"
#include "containers/Container.hpp"


namespace morda{

class List : public Container{
	size_t addedIndex = size_t(-1);
	
	size_t posIndex = 0;
	real posOffset = real(0);
	
	bool isVertical;
	
	size_t numTailItems = 0;//Zero means that number of tail items has to be recomputed
	size_t firstTailItemIndex = 0;
	real firstTailItemOffset = real(0);
	
	
protected:
	List(bool isVertical, const stob::Node* chain);
public:
	List(const List&) = delete;
	List& operator=(const List&) = delete;
	
	
	class ItemsProvider : virtual public ting::Shared{
		friend class List;
		
		List* list = nullptr;
	protected:
		ItemsProvider(){}
	public:
		
		virtual size_t count()const noexcept = 0;
		
		virtual std::shared_ptr<Widget> getWidget(size_t index) = 0;
		
		virtual void recycle(size_t index, std::shared_ptr<Widget> w){}
		
		void notifyDataSetChanged();
	};
	
	void setItemsProvider(std::shared_ptr<ItemsProvider> provider = nullptr);
	

	void layOut()override;
	
	morda::Vec2r measure(const morda::Vec2r& quotum) const override;

	
	size_t visibleCount()const{
		return this->children().size();
	}
	
	void setScrollPosAsFactor(real factor);
	
	real scrollFactor()const noexcept;
	
	std::function<void(List&)> dataSetChanged;
	
private:
	std::shared_ptr<ItemsProvider> provider;
	
	void updateChildrenList();
	
	bool arrangeWidget(std::shared_ptr<Widget>& w, real& pos, bool add, size_t index, T_ChildrenList::const_iterator insertBefore);//returns true if it was the last visible widget
	
	void updateTailItemsInfo();
	
	void handleDataSetChanged();
};



class HorizontalList : public List{
public:
	HorizontalList(const stob::Node* chain = nullptr) :
			Widget(chain),
			List(false, chain)
	{}
	
	HorizontalList(const HorizontalList&) = delete;
	HorizontalList& operator=(const HorizontalList&) = delete;
};


class VerticalList : public List{
public:
	VerticalList(const stob::Node* chain = nullptr) :
			Widget(chain),
			List(true, chain)
	{}
	
	VerticalList(const VerticalList&) = delete;
	VerticalList& operator=(const VerticalList&) = delete;
};

}
