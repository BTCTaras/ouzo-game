#pragma once

#include <memory>
#include <string>
#include <map>

class CShader {
public:
    ///
    /// Loads and compiles a shader from a file.
    ///
    virtual void LoadFromFile(const char *file) = 0;

    ///
    /// Loads and compiles a shader from memory.
    /// The chunk name is the name to be used in the cache and in error messages.
    ///
    virtual void Load(const char *chunkName, const char *source) = 0;

    ///
    /// Gets the name of this shader used in error messages.
    /// This is often the filename.
    ///
    virtual const std::string& GetChunkName() = 0;

    ///
    /// Increments the attachment counter in order to detect
    /// invalid shader deletion.
    ///
    virtual void OnAttach() = 0;

    ///
    /// Decrements the attachment counter.
    ///
    virtual void OnDetach() = 0;
};

typedef std::shared_ptr<CShader> S_CShader;

class CGLShader : public CShader {
public:
  CGLShader(unsigned int type);
  CGLShader(unsigned int type, const char *file);
  ~CGLShader();

  virtual void LoadFromFile(const char *file) override;
  virtual void Load(const char *chunkName, const char *source) override;
  virtual const std::string& GetChunkName() override;
  virtual void OnAttach() override;
  virtual void OnDetach() override;

  ///
  /// Returns the OpenGL handle to the shader.
  ///
  unsigned int GetOpenGLHandle();

private:
  ///
  /// Compiles this shader.
  ///
  void Compile();

  unsigned int m_id;
  unsigned int m_attachmentCounter;

  std::string m_chunkName;

};

typedef std::shared_ptr<CGLShader> S_CGLShader;

class CProgram {
public:
    ///
    /// Loads a program from the specified shaders.
    /// The count must specify how many shaders are given, *not* the size.
    ///
    virtual void LoadFromShaders(size_t count, S_CShader *shaders) = 0;

    ///
    /// Same as calling glUseProgram.
    ///
    virtual void Use() = 0;

};

typedef std::shared_ptr<CProgram> S_CProgram;

class CGLProgram : public CProgram {
public:
  CGLProgram();
  CGLProgram(size_t count, S_CShader *shaders);
  ~CGLProgram();

  virtual void LoadFromShaders(size_t count, S_CShader *shaders) override;

  virtual void Use() override;

  ///
  /// Returns the OpenGL handle to the program.
  ///
  unsigned int GetOpenGLHandle();

  ///
  /// Retrieves a uniform location from the uniform cache.
  ///
  unsigned int GetUniformLocation(const char *name, bool ignoreUniformNotFound = false);

private:
    void Link();

    std::map<std::string, unsigned int> m_uniformCache;
    unsigned int m_id;
};

typedef std::shared_ptr<CGLProgram> S_CGLProgram;
