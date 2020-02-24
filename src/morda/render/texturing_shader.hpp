#pragma once

#include <utki/Buf.hpp>
#include <utki/Unique.hpp>
#include <r4/vector2.hpp>

#include "texture_2d.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "vertex_array.hpp"

namespace morda{


class texturing_shader{
public:
	texturing_shader(){}
	
	texturing_shader(const texturing_shader&) = delete;
	texturing_shader& operator=(const texturing_shader&) = delete;
	
	virtual ~texturing_shader()noexcept{}
	
	virtual void render(const r4::mat4f &m, const morda::vertex_array& va, const texture_2d& tex)const = 0;
private:

};

}
