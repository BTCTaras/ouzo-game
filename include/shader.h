#pragma once

#include <memory>
#include <string>

class CShader {
    friend class CProgram;

public:
    CShader(unsigned int type);
    CShader(unsigned int type, const char *file);
    ~CShader();

    ///
    /// Loads and compiles a shader from a file.
    ///
    void LoadFromFile(const char *file);

    ///
    /// Loads and compiles a shader from memory.
    /// The chunk name is the name to be used in the cache and in error messages.
    ///
    void Load(const char *chunkName, const char *source);

    ///
    /// Returns the OpenGL handle to the shader.
    ///
    unsigned int GetHandle();
    const std::string& GetChunkName();

private:
    ///
    /// Compiles this shader.
    ///
    void Compile();

    ///
    /// Increments the attachment counter in order to detect
    /// invalid shader deletion.
    ///
    void OnAttach();

    unsigned int m_id;
    unsigned int m_attachmentCounter;

    std::string m_chunkName;
};

typedef std::shared_ptr<CShader> S_CShader;

class CProgram {
public:
  CProgram(size_t count, S_CShader *shaders);
  ~CProgram();
};

typedef std::shared_ptr<CProgram> S_CProgram;
