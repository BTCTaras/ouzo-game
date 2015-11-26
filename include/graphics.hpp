#pragma once

#include "shader.hpp"
#include "texture.hpp"
#include "atlas_factory.hpp"
#include "buffer.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <SDL2/SDL.h>

///
/// Represents a vertex. Values in order (floats): x, y, z, u, v.
///
typedef struct {
	float x, y, z;
	float u, v;
} vertex_t;

typedef struct {
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
} mvp_matrix_t;

typedef struct {
	float r, g, b;
} colour_t;

enum ShaderType {
	VERTEX_SHADER, FRAGMENT_SHADER
};

enum PrimitiveType {
	POINTS,
	LINE_STRIP,
	LINE_LOOP,
	LINES,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,
	TRIANGLES,
};

class CGraphics {
public:
	virtual void Init(SDL_Window *window) = 0;

	///
	/// To be called at the beginning of a frame. Behaviour is
	/// backend specific.
	///
	virtual void BeginScene() = 0;

	///
	/// Enables all vertex attributes and passes them to the current program.
	///
	/// \param[in]	mvp				The MVP matrix to use for rendering.
	/// \param[in]	vertexBuffer	The vertex buffer to use for rendering.
	/// \param[in]	program			The program to use for rendering.
	///
	virtual void Begin(mvp_matrix_t &mvp, S_CBuffer vertexBuffer, S_CProgram program = nullptr) = 0;

	///
	/// Draws a model. The vertex buffer is specified by CGraphics::Begin.
	///
	/// \param[in]  primitive     Specifies how the vertices shall be connected.
	/// \param[in]  elementBuffer The buffer containing the model indices.
	///
	virtual void Draw(PrimitiveType primitive, S_CBuffer elementBuffer = nullptr) = 0;

	///
	/// Sets the texture to draw with.
	///
	/// \param[in]  tex   The texture to use.
	/// \param[in]  slot  What slot to assign it to. Used for multitexturing.
	///
	virtual void SetTexture(S_CTexture tex = nullptr, unsigned int slot = 0) = 0;

	///
	/// Disables all vertex attributes *and unbinds any bound programs*.
	///
	virtual void End() = 0;

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

	virtual unsigned int GetMaxTextureSize() = 0;
};

typedef std::shared_ptr<CGraphics> S_CGraphics;

class CGLGraphics : public CGraphics {
public:
	~CGLGraphics();

	virtual void Init(SDL_Window *window) override;
	virtual void BeginScene() override;
	virtual void Begin(mvp_matrix_t &mvp, S_CBuffer vertexBuffer, S_CProgram program = nullptr) override;
	virtual void End() override;
	virtual void EndScene() override;

	virtual S_CProgram GetDefaultProgram() override;
	virtual S_CTexture CreateTexture(const char *file = NULL) override;
	virtual S_CShader CreateShader(ShaderType type, const char *file = NULL) override;
	virtual S_CProgram CreateProgram(size_t count, S_CShader *shaders) override;
	virtual S_CAtlasFactory CreateAtlasFactory(unsigned int width, unsigned int height, unsigned int channels) override;
	virtual S_CBuffer CreateBuffer(BufferType type, BufferStorageType storageType = BufferStorageType::STATIC) override;
	virtual void Draw(PrimitiveType primitive, S_CBuffer elementBuffer = nullptr) override;
	virtual void SetTexture(S_CTexture tex = nullptr, unsigned int slot = 0) override;

	virtual unsigned int GetMaxTextureSize() override;

	///
	/// The vertex attribute containing the vertex position.
	///
	static const unsigned int VERT_ATTRIB_POS;

	///
	/// The vertex atrtribute containing the texture coordinates.
	///
	static const unsigned int VERT_ATTRIB_TEX_COORDS;

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

	unsigned int m_glMaxTextureSize;
};

typedef std::shared_ptr<CGLGraphics> S_CGLGraphics;
