#ifdef _WIN32

#include "graphics.hpp"
#include "d3d/d3d_graphics.hpp"

CD3DMatrix::CD3DMatrix(D3DXMATRIX *mat) {
	if (mat != nullptr) {
		m_internalMat = *mat;
	} else {
		D3DXMatrixIdentity(&m_internalMat);
	}
}

void CD3DMatrix::Perspective(float fov, float width, float height, float nearZ, float farZ) {
	D3DXMatrixPerspectiveFovRH(&m_internalMat, D3DXToRadian(fov), width / height, nearZ, farZ);
}

void CD3DMatrix::Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ) {
	D3DXMatrixOrthoOffCenterRH(&m_internalMat, left, right, bottom, top, nearZ, farZ);
}

void CD3DMatrix::Translate(float x, float y, float z) {
	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, x, y, -z);
	m_internalMat *= translation;
}

void CD3DMatrix::Rotate(float x, float y, float z) {
	D3DXMATRIX rotation;
	// We have to do adventurous things like this because D3D is stupid.
	D3DXMatrixRotationYawPitchRoll(
		&rotation,
		D3DXToRadian(y),
		D3DXToRadian(x),
		D3DXToRadian(z)
	);

	m_internalMat *= rotation;
}

void CD3DMatrix::Scale(float x, float y, float z) {
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, x, y, z);
	m_internalMat *= scale;
}

void CD3DMatrix::LoadIdentity() {
	D3DXMatrixIdentity(&m_internalMat);
}

S_CMatrix CD3DMatrix::operator*(S_CMatrix other) {
	S_CD3DMatrix d3d_other = std::static_pointer_cast<CD3DMatrix>(other);
	return S_CD3DMatrix(
		new CD3DMatrix(&(m_internalMat * d3d_other->m_internalMat))
	);
}

D3DXMATRIX* CD3DMatrix::GetD3DXHandle() {
	return &m_internalMat;
}

#endif