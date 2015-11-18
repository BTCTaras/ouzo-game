#pragma once

#include <memory>
#include <string>
#include <map>

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

    ///
    /// Decrements the attachment counter.
    ///
    void OnDetach();

    unsigned int m_id;
    unsigned int m_attachmentCounter;

    std::string m_chunkName;
};

typedef std::shared_ptr<CShader> S_CShader;

class CProgram {
public:
    CProgram();
    CProgram(size_t count, S_CShader *shaders);
    ~CProgram();

    ///
    /// Loads a program from the specified shaders.
    /// The count must specify how many shaders are given, *not* the size.
    ///
    void LoadFromShaders(size_t count, S_CShader *shaders);

    ///
    /// Returns the OpenGL handle to the program.
    ///
    unsigned int GetHandle();

    ///
    /// Retrieves a uniform location from the uniform cache.
    ///
    unsigned int GetUniformLocation(const char *name);

    ///
    /// Same as calling glUseProgram.
    ///
    void Use();

private:
    void Link();

    std::map<std::string, unsigned int> m_uniformCache;

    unsigned int m_id;
};

typedef std::shared_ptr<CProgram> S_CProgram;
