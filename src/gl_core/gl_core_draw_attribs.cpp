#include "graphics_common/draw_attribs.hpp"

#include <GL/glew.h>

#include "graphics_common/buffer.hpp"

CGLDrawAttribs::CGLDrawAttribs() {
	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);

	glVertexAttribDivisor((GLuint)AttribType::OFFSET1, 1);
	glVertexAttribDivisor((GLuint)AttribType::OFFSET2, 1);
	glVertexAttribDivisor((GLuint)AttribType::OFFSET3, 1);
	glVertexAttribDivisor((GLuint)AttribType::OFFSET4, 1);
}

CGLDrawAttribs::~CGLDrawAttribs() {
	glDisableVertexAttribArray((GLuint)AttribType::POSITION);
	glDisableVertexAttribArray((GLuint)AttribType::TEX_COORDS);
	glDisableVertexAttribArray((GLuint)AttribType::NORMAL);
	glDeleteVertexArrays(1, &m_id);
}

static const GLint ATTRIB_POSITION_COUNT = 4;
static const GLint ATTRIB_TEX_COORDS_COUNT = 2;
static const GLint ATTRIB_NORMAL_COUNT = 3;
static const GLint ATTRIB_OFFSET_COUNT = 4;

void CGLDrawAttribs::SetSource(AttribType type, S_CBuffer buf, size_t offset) {
	glBindVertexArray(m_id);

	S_CGLBuffer gl_buf = std::static_pointer_cast<CGLBuffer>(buf);
	glBindBuffer(GL_ARRAY_BUFFER, gl_buf->GetOpenGLHandle());

	GLint count;

	switch (type) {
	case AttribType::POSITION:
		count = ATTRIB_POSITION_COUNT;
		break;

	case AttribType::TEX_COORDS:
		count = ATTRIB_TEX_COORDS_COUNT;
		break;

	case AttribType::NORMAL:
		count = ATTRIB_NORMAL_COUNT;
		break;

	case AttribType::OFFSET1:
	case AttribType::OFFSET2:
	case AttribType::OFFSET3:
	case AttribType::OFFSET4:
		count = ATTRIB_OFFSET_COUNT;
		break;
	}

	glEnableVertexAttribArray((GLuint)type);
	glVertexAttribPointer(
		(GLuint)type,
		count,
		GL_FLOAT,
		GL_FALSE,
		gl_buf->GetElementSize(),
		(void*)offset
	);

	m_buffers[type] = buf;
}

S_CBuffer CGLDrawAttribs::GetSource(AttribType type) {
	if (m_buffers.find(type) == m_buffers.end()) {
		return nullptr;
	}

	return m_buffers[type];
}

unsigned int CGLDrawAttribs::GetOpenGLHandle() {
	return m_id;
}
