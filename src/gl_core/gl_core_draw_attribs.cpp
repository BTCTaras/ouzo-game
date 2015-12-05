#include "graphics_common/draw_attribs.hpp"

#include <GL/glew.h>

#include "graphics_common/buffer.hpp"

CGLDrawAttribs::CGLDrawAttribs() {
	glGenVertexArrays(1, &m_id);
}

CGLDrawAttribs::~CGLDrawAttribs() {
	glDisableVertexAttribArray((GLuint)AttribType::POSITION);
	glDisableVertexAttribArray((GLuint)AttribType::TEX_COORDS);
	glDisableVertexAttribArray((GLuint)AttribType::NORMAL);
	glDisableVertexAttribArray((GLuint)AttribType::INSTANCE);
	glDeleteVertexArrays(1, &m_id);
}

const GLint ATTRIB_POSITION_COUNT = 4;
const GLint ATTRIB_TEX_COORDS_COUNT = 2;
const GLint ATTRIB_NORMAL_COUNT = 3;

void CGLDrawAttribs::SetSource(AttribType type, S_CBuffer buf, size_t offset) {
	glBindVertexArray(m_id);

	S_CGLBuffer gl_buf = std::static_pointer_cast<CGLBuffer>(buf);
	glBindBuffer(GL_ARRAY_BUFFER, gl_buf->GetOpenGLHandle());

	GLint count;

	switch (type) {
	case AttribType::POSITION:
		count = ATTRIB_NORMAL_COUNT;
		break;

	case AttribType::TEX_COORDS:
		count = ATTRIB_TEX_COORDS_COUNT;
		break;

	case AttribType::NORMAL:
		count = ATTRIB_NORMAL_COUNT;
		break;

    case AttribType::INSTANCE:
        count = 1;
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

	if (type == AttribType::INSTANCE) {
        glVertexAttribDivisor((GLuint)type, 1);
	}
}

unsigned int CGLDrawAttribs::GetOpenGLHandle() {
	return m_id;
}
