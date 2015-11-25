#pragma once

#include <memory>

enum BufferStorageType {
  STATIC, DYNAMIC
};

enum BufferType {
  VERTEX_BUFFER,
  ELEMENT_BUFFER,
  DRAW_COMMAND_BUFFER
};

class CBuffer {
public:
  CBuffer(BufferType type, BufferStorageType storageType = BufferStorageType::STATIC);

  ///
  /// Fills the buffer with the specified data, overwriting any existing data.
  ///
  /// \param[in]  dataSize  The size of the inputted data in bytes.
  /// \param[in]  data      The data to save into the buffer.
  ///
  virtual void Orphan(size_t dataSize, void *data) = 0;

  ///
  /// Gets the size of the buffer in bytes.
  ///
  virtual size_t GetSize() = 0;

  ///
  /// Returns the type of the buffer.
  ///
  BufferType GetType();

  ///
  /// Returns the storage type of the buffer.
  ///
  BufferStorageType GetStorageType();

protected:
  BufferType m_type;
  BufferStorageType m_storageType;
};

typedef std::shared_ptr<CBuffer> S_CBuffer;

class CGLBuffer : public CBuffer {
public:
  CGLBuffer(BufferType type, BufferStorageType storageType = BufferStorageType::STATIC);
  ~CGLBuffer();

  virtual void Orphan(size_t dataSize, void *data) override;
  virtual size_t GetSize() override;

  ///
  /// Returns the OpenGL handle of this buffer.
  ///
  unsigned int GetOpenGLHandle();

  ///
  /// Returns the given buffer type as an OpenGL enum.
  ///
  /// E.g:  BufferType::VERTEX_BUFFER       -> GL_ARRAY_BUFFER
  ///       BufferType::ELEMENT_BUFFER      -> GL_ELEMENT_BUFFER
  ///       BufferType::DRAW_COMMAND_BUFFER -> GL_DRAW_INDIRECT_BUFFER
  ///
  static unsigned int GetOpenGLBufferTypeEnum(BufferType type);

  ///
  /// Returns the given buffer storage type as an OpenGL enum.
  ///
  /// E.g:  BufferStorageType::STATIC   -> GL_STATIC_DRAW
  ///       BufferStorageType::DYNAMIC  -> GL_DYNAMIC_DRAW / GL_STREAM_DRAW
  ///
  static unsigned int GetOpenGLBufferStorageTypeEnum(BufferStorageType type);

private:
  unsigned int m_id;
  size_t m_size;
};

typedef std::shared_ptr<CGLBuffer> S_CGLBuffer;
