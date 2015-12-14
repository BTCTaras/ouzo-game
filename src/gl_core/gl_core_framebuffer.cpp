#include "graphics_common/framebuffer.hpp"

#include <GL/glew.h>

CGLFrameBuffer::CGLFrameBuffer() {
	glGenFramebuffers(1, &m_id);
}

CGLFrameBuffer::~CGLFrameBuffer() {
	glDeleteFramebuffers(1, &m_id);
}

void CGLFrameBuffer::Bind(short purpose) {
	if ((purpose & GFX::BIND_READ) == GFX::BIND_READ) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
	}

	if ((purpose & GFX::BIND_WRITE) == GFX::BIND_WRITE) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
	}
}

void CGLFrameBuffer::AssignToTexture(S_CTexture tex) {
	
}
