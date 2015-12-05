#pragma once

#include <glm/glm.hpp>
#include <memory>

class CMatrix;
typedef std::shared_ptr<CMatrix> S_CMatrix;

class CMatrix {
public:
	virtual void Perspective(float fov, float width, float height, float nearZ, float farZ) = 0;
	virtual void Orthographic(float left, float right, float bottom, float top, float nearZ = -1.0f, float farZ = 1.0f) = 0;
	virtual void Translate(float x, float y, float z = 0.0f) = 0;
	virtual void Rotate(float x, float y, float z) = 0;
	virtual void Scale(float x, float y, float z) = 0;
	virtual void LoadIdentity() = 0;

	virtual float* ValuePointer() = 0;

	virtual S_CMatrix operator*(S_CMatrix other) = 0;
};

S_CMatrix operator*(S_CMatrix first, S_CMatrix second);

typedef struct {
	S_CMatrix projection;
	S_CMatrix view;
	S_CMatrix model;
} mvp_matrix_t;

class CGLMatrix : public CMatrix {
public:
	CGLMatrix(glm::mat4 *mat = nullptr);

	virtual void Perspective(float fov, float width, float height, float nearZ, float farZ) override;
	virtual void Orthographic(float left, float right, float bottom, float top, float nearZ = -1.0f, float farZ = 1.0f) override;
	virtual void Translate(float x, float y, float z = 0.0f) override;
	virtual void Rotate(float x, float y, float z) override;
	virtual void Scale(float x, float y, float z) override;
	virtual void LoadIdentity() override;

	virtual S_CMatrix operator*(S_CMatrix other) override;

	virtual float* ValuePointer() override;
	glm::mat4* GetGLMMatrix();

private:
	glm::mat4 m_internalMat;
};

typedef std::shared_ptr<CGLMatrix> S_CGLMatrix;
