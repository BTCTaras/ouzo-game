#include "buffer.hpp"

#include <GL/glew.h>

CGLBuffer::CGLBuffer(BufferType type, BufferStorageType storageType)
  : CBuffer(type, storageType),
    m_id(0)
{
  glGenBuffers(1, &m_id);
}

CGLBuffer::~CGLBuffer() {
  if (m_id > 0) {
    glDeleteBuffers(1, &m_id);
  }
}

void CGLBuffer::Orphan(size_t dataSize, void *data) {
  GLenum typeEnum = CGLBuffer::GetOpenGLBufferTypeEnum(m_type);
  glBindBuffer(typeEnum, m_id);

  GLenum storageTypeEnum = CGLBuffer::GetOpenGLBufferStorageTypeEnum(m_storageType);
  glBufferData(typeEnum, dataSize, data, storageTypeEnum);

  m_size = dataSize;
}

size_t CGLBuffer::GetSize() {
  return m_size;
}

unsigned int CGLBuffer::GetOpenGLHandle() {
  return m_id;
}

unsigned int CGLBuffer::GetOpenGLBufferTypeEnum(BufferType type) {
  switch (type) {
    case BufferType::VERTEX_BUFFER:
      return GL_ARRAY_BUFFER;

    case BufferType::ELEMENT_BUFFER:
      return GL_ELEMENT_ARRAY_BUFFER;

    case BufferType::DRAW_COMMAND_BUFFER:
      return GL_DRAW_INDIRECT_BUFFER;

    default:
      return GL_FALSE;
  }
}

unsigned int CGLBuffer::GetOpenGLBufferStorageTypeEnum(BufferStorageType storageType) {
  switch (storageType) {
    case BufferStorageType::STATIC:
      return GL_STATIC_DRAW;

    case BufferStorageType::DYNAMIC:
      return GL_DYNAMIC_DRAW;

    default:
      return GL_FALSE;
  }
}
