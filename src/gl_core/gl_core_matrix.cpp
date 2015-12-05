#include "graphics.hpp"
#include "matrix.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

CGLMatrix::CGLMatrix(glm::mat4 *mat) {
	if (mat != nullptr) {
		m_internalMat = *mat;
	} else {
		m_internalMat = glm::mat4(1.0f);
	}
}

void CGLMatrix::Perspective(float fov, float width, float height, float nearZ, float farZ) {
	m_internalMat *= glm::perspectiveFov(fov, width, height, nearZ, farZ);
}

void CGLMatrix::Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ) {
	m_internalMat *= glm::ortho(left, right, bottom, top, nearZ, farZ);
}

void CGLMatrix::Translate(float x, float y, float z) {
	m_internalMat = glm::translate(m_internalMat, glm::vec3(x, y, z));
}

void CGLMatrix::Rotate(float x, float y, float z) {
	m_internalMat = glm::rotate(m_internalMat, x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_internalMat = glm::rotate(m_internalMat, y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_internalMat = glm::rotate(m_internalMat, z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void CGLMatrix::Scale(float x, float y, float z) {
	m_internalMat = glm::scale(m_internalMat, glm::vec3(x, y, z));
}

void CGLMatrix::LoadIdentity() {
	m_internalMat = glm::mat4(1.0f);
}

S_CMatrix CGLMatrix::operator*(S_CMatrix other) {
	S_CGLMatrix gl_other = std::static_pointer_cast<CGLMatrix>(other);
	glm::mat4 m = m_internalMat * gl_other->m_internalMat;
	return S_CMatrix(new CGLMatrix(&m));
}

glm::mat4* CGLMatrix::GetGLMMatrix() {
	return &m_internalMat;
}

float* CGLMatrix::ValuePointer() {
	return glm::value_ptr(m_internalMat);
}
