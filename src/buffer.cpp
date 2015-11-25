#include "buffer.hpp"

CBuffer::CBuffer(BufferType type, BufferStorageType storageType) {
  m_type = type;
  m_storageType = storageType;
}

BufferType CBuffer::GetType() {
  return m_type;
}

BufferStorageType CBuffer::GetStorageType() {
  return m_storageType;
}
