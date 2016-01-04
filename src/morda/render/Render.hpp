/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include <memory>

#include <utki/Buf.hpp>
#include <utki/Void.hpp>

#include <kolme/Matrix4.hpp>
#include <kolme/Rectangle.hpp>


namespace morda{


class Shader;


class Render{
	friend class Shader;
	friend class Texture2D;
	friend class FrameBuffer;
	
	friend class App;
	
	std::unique_ptr<utki::Void> pimpl;
	
	Render();
	~Render()noexcept;
	
public:
	
	enum class EMode{
		//NOTE: do not change order!!!
		
		TRIANGLES,
		TRIANGLE_FAN,
		LINE_LOOP
	};
	
	class InputID{
		friend class morda::Render;
		std::int64_t id;
		void* data;
	public:
		InputID(std::int64_t id, void* data = nullptr) :
			id(id),
			data(data)
		{}
	};
	
	//=== functions to be used by Shader class internally
private:
	static void renderArrays(EMode mode, size_t numElements);
	
	static void renderElements(EMode mode, const utki::Buf<std::uint16_t>& i);
	
	//returns pointer to shader object
	static std::unique_ptr<utki::Void> compileShader(const char* vertexShaderCode, const char* fragmentShaderCode);
	
	static void bindShader(utki::Void& p);
	
	static InputID getAttribute(utki::Void& p, const char* n);
	
	static InputID getUniform(utki::Void& p, const char* n);
	
	static void setUniformMatrix4f(InputID id, const kolme::Matr4f& m);
	
	static void setUniform1i(InputID id, int i);
	
	static void setUniform2f(InputID id, kolme::Vec2f v);
	
	static void setUniform4f(InputID id, float x, float y, float z, float a);
	
	static void setUniform4f(InputID id, const utki::Buf<kolme::Vec4f> v);
	
	static void setVertexAttribArray(InputID id, const kolme::Vec3f* a);
	
	static void setVertexAttribArray(InputID id, const kolme::Vec2f* a);
	//=== ~~~
	
	
public:
	
	static void setViewport(kolme::Recti r);
	
	static kolme::Recti getViewport();
	
	static void clearColor(kolme::Vec4f c = kolme::Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
	
	static void clearDepth(float d = 0.0f);
	
	static void clearStencil(std::uint8_t v = 0);
	
	static void clearAccumulator(kolme::Vec4f c = kolme::Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
	
	static bool isScissorEnabled();
	
	static kolme::Recti getScissorRect();
	
	static void setScissorEnabled(bool enabled);
	
	static void setScissorRect(kolme::Recti r);
	
	static unsigned getMaxTextureSize();
	
	enum class ETexFilter{
		//NOTE: do not change the order!!!
		
		NEAREST,
		LINEAR
	};
	
	static void setCullEnabled(bool enable);
	
	//=== functions to be used by Texture class
private:
	static std::unique_ptr<utki::Void> create2DTexture(kolme::Vec2ui dim, unsigned numChannels, const utki::Buf<std::uint8_t> data, ETexFilter minFilter, ETexFilter magFilter);
	
	static void bindTexture(utki::Void& tex, unsigned unitNum);
	//=== ~~~
	
public:
	
	static void unbindTexture(unsigned unitNum);
	
	static void copyColorBufferToTexture(kolme::Vec2i dst, kolme::Recti src);

	static void swapFrameBuffers();
	
	//=== functions to be used by FrameBuffer class
private:
	static std::unique_ptr<utki::Void> createFrameBuffer();
	
	static void bindFrameBuffer(utki::Void* fbo);
	
	static void attachColorTexture2DToFrameBuffer(utki::Void* tex);
	//=== ~~~
	
	
public:
	static void setBlendEnabled(bool enable);
	
	enum class EBlendFactor{
		//NOTE: do not change order
		
		ONE,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		ONE_MINUS_DST_ALPHA
	};
	
	static void setBlendFunc(EBlendFactor srcClr, EBlendFactor dstClr, EBlendFactor srcAlpha, EBlendFactor dstAlpha);

	static bool isBoundFrameBufferComplete();

};



}
