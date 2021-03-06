#pragma once

#include "shader.hpp"
#include "texture.hpp"
#include "atlas_factory.hpp"
#include "buffer.hpp"
#include "draw_attribs.hpp"
#include "matrix.hpp"
#include "instance_data.hpp"

#include <memory>
#include <SDL2/SDL.h>

///
/// Represents a vertex. Values in order (floats): x, y, z, w, u, v, nx, ny, nz.
///
struct vertex_t {
	float x, y, z, w;
	float u, v;
	float nx, ny, nz;
};

///
/// Represents a colour. Values in order (floats): r, g, b.
///
struct colour_t {
	float r, g, b;
};

enum PrimitiveType {
	GFX_POINTS,
	GFX_LINE_STRIP,
	GFX_LINE_LOOP,
	GFX_LINES,
	GFX_TRIANGLE_STRIP,
	GFX_TRIANGLE_FAN,
	GFX_TRIANGLES,
};

class CGraphics {
public:
	virtual void Init(SDL_Window *window) = 0;

	///
	/// To be called at the beginning of a frame. Behaviour is
	/// backend specific, though includes clearing the framebuffer.
	///
	virtual void BeginScene() = 0;

    ///
    /// Sets the attributes to draw with.
    ///
	virtual void SetDrawAttributes(S_CDrawAttribs attribs) = 0;

    ///
    /// Sets the program to draw with.
    ///
	virtual void SetDrawProgram(S_CProgram program) = 0;

    ///
    /// Sets the transform to draw with.
    ///
	virtual void SetDrawTransform(mvp_matrix_t &mvp) = 0;

    ///
    /// Sets the buffer to draw with.
    ///
	virtual void SetDrawBuffer(S_CBuffer buffer) = 0;

	///
	/// Sets the texture to draw with.
	///
	/// \param[in]  tex   The texture to use.
	/// \param[in]  slot  What slot to assign it to. Used for multitexturing.
	///
	virtual void SetDrawTexture(S_CTexture tex = nullptr, unsigned int slot = 0) = 0;

	///
	/// Draws a model. The vertex buffer is specified by CGraphics::Begin.
	///
	/// \param[in]  primitive     Specifies how the vertices shall be connected.
	/// \param[in]  elementBuffer The buffer containing the model indices.
	///
	virtual void Draw(PrimitiveType primitive, S_CBuffer elementBuffer = nullptr) = 0;

    ///
    /// Draws a model multiple times.
    /// Uses the AttribType::INSTANCE attribute to determine per-model transformations.
    ///
    /// \param[in]  primitive       Specifies how the vertices shall be connected.
	/// \param[in]	instanceCount	The amount of instances to render.
    /// \param[in]  elementBuffer   The buffer containing the model indices.
	///
    virtual void DrawInstanced(PrimitiveType primitive, size_t instanceCount, S_CBuffer elementBuffer = nullptr) = 0;

	///
	/// Finishes off a frame. Behaviour is backend specific, though
	/// swapping the buffers is essentially guaranteed.
	///
	virtual void EndScene() = 0;

	///
	/// Returns a basic program that applies the diffuse material to a shape.
	///
	virtual S_CProgram GetDefaultProgram() = 0;

	///
	/// Returns a basic texture object that works with this renderer.
	/// \param[in]  file  Load the texture from a file, at the same time, too. Can be NULL.
	///
	virtual S_CTexture CreateTexture(const char *file = NULL) = 0;

	///
	/// Loads a shader from a file.
	/// File names must be specified without their extension.
	/// The function will decide the final extension, usually, it'll be the typical
	/// shader extension for the backend in question prefixed by the shader type.
	///
	/// E.g.    OpenGL:   <name>.frag.glsl / <name>.vert.glsl
	///         Direct3D: <name>.frag.hlsl / <name>.vert.hlsl
	///
	/// \param[in]
	/// \param[in]  file  The file to load this shader from.
	///
	virtual S_CShader CreateShader(ShaderType type, const char *file = NULL) = 0;

	///
	/// Creates a program from several shader objects.
	///
	/// \param[in]  count   The amount of shaders passed into this function.
	/// \param[in]  shaders The shaders to create this program with.
	///
	virtual S_CProgram CreateProgram(size_t count, S_CShader *shaders) = 0;

	///
	/// Creates an atlas factory.
	///
	/// \param[in]  width     The width of the atlases to produce.
	/// \param[in]  height    The height of the atlases to produce.
	/// \param[in]  channels  The amount of channels the atlas should contain.
	///
	virtual S_CAtlasFactory CreateAtlasFactory(unsigned int width, unsigned int height, unsigned int channels) = 0;

	///
	/// Creates a generic buffer stored in video memory.
	///
	/// \param[in]  type        Specifies what this buffer contains.
	/// \param[in]  storageType Specifies how the data stored in this buffer is accessed.
	///
	virtual S_CBuffer CreateBuffer(BufferType type, BufferStorageType storageType = BufferStorageType::STATIC) = 0;

	///
	/// Creates an identity matrix.
	///
	virtual S_CMatrix CreateIdentityMatrix() = 0;

    ///
    /// Creates a simple uninitialised S_CDrawAttribs object.
    ///
	virtual S_CDrawAttribs CreateDrawAttribs() = 0;

    ///
    /// Creates a S_CDrawAttribs with its sources set to standardised values.
    /// Passing in an array of vertex_t will definitely work.
    ///
    /// That is:
    ///
    /// POSITION has offset 0 bytes (4 floats)
    /// TEX_COORDS has offset 16 bytes (2 floats)
    /// NORMAL has offset 24 bytes (3 floats)
    ///
	virtual S_CDrawAttribs CreateDrawAttribs(S_CBuffer buffer) = 0;

	///
	///	Defines the area that this CGraphics object shall render to.
	///
	///	\param[in]	x	The x coordinate of the viewport, relative to the window.
	/// \param[in]	y	The y coordinate of the viewport, relative to the window.
	/// \param[in]	w	The width of the viewport.
	/// \param[in]	h	The height of the viewport.
	///
	virtual void SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h) = 0;

	virtual unsigned int GetMaxTextureSize() = 0;

	virtual int GetMaxInstanceCount() = 0;
};

typedef std::shared_ptr<CGraphics> S_CGraphics;

class CGLGraphics : public CGraphics {
public:
	~CGLGraphics();

	virtual void Init(SDL_Window *window) override;
	virtual void BeginScene() override;
	virtual void EndScene() override;

	virtual void SetDrawAttributes(S_CDrawAttribs attribs) override;
	virtual void SetDrawProgram(S_CProgram program) override;
	virtual void SetDrawTexture(S_CTexture tex = nullptr, unsigned int slot = 0) override;
	virtual void SetDrawTransform(mvp_matrix_t &mvp) override;
	virtual void SetDrawBuffer(S_CBuffer buffer) override;

	virtual void Draw(PrimitiveType primitive, S_CBuffer elementBuffer = nullptr) override;
	virtual void DrawInstanced(PrimitiveType primitive, size_t instanceCount, S_CBuffer elementBuffer = nullptr) override;

	virtual S_CProgram GetDefaultProgram() override;
	virtual S_CTexture CreateTexture(const char *file = NULL) override;
	virtual S_CShader CreateShader(ShaderType type, const char *file = NULL) override;
	virtual S_CProgram CreateProgram(size_t count, S_CShader *shaders) override;
	virtual S_CAtlasFactory CreateAtlasFactory(unsigned int width, unsigned int height, unsigned int channels) override;
	virtual S_CBuffer CreateBuffer(BufferType type, BufferStorageType storageType = BufferStorageType::STATIC) override;
	virtual S_CMatrix CreateIdentityMatrix() override;
	virtual S_CDrawAttribs CreateDrawAttribs() override;
	virtual S_CDrawAttribs CreateDrawAttribs(S_CBuffer buffer) override;

	virtual void SetViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h) override;

	virtual unsigned int GetMaxTextureSize() override;

	virtual int GetMaxInstanceCount() override;

	///
	/// Converts the given PrimitiveType to an OpenGL enum.
	///
	/// \param[in]  type  The primitive type to convert.
	///
	static unsigned int GetOpenGLPrimitiveTypeEnum(PrimitiveType type);

private:
	unsigned int m_vao; // Stores the vertex attributes. Needed for OpenGL 3.0
	S_CProgram m_defaultProgram;
	SDL_GLContext m_context;
	SDL_Window *m_window;
	S_CBuffer m_currentBuffer;
	CInstanceData m_defaultInstData;
	S_CGLProgram m_drawProgram;

	unsigned int m_glMaxTextureSize;
	unsigned int m_lastActiveTex;
};

typedef std::shared_ptr<CGLGraphics> S_CGLGraphics;
