#include "Texture2D.hpp"



using namespace morda;




void Texture2D::Constructor(Vec2ui d, unsigned numChannels, const utki::Buf<std::uint8_t> data, Render::ETexFilter minFilter, Render::ETexFilter magFilter) {
	this->dim = d.to<real>();

	this->tex = Render::create2DTexture(d, numChannels, data, minFilter, magFilter);
}
