#include "scene/scene_benchmark.hpp"

#include "graphics_common/wavefront.hpp"
#include "graphics_common/graphics.hpp"
#include "graphics_common/font.hpp"
#include "game.hpp"

#include <random>
#include <sstream>

CSceneBenchmark::CSceneBenchmark() : m_starCount(0) {
}

void CSceneBenchmark::OnInit() {
	m_buffer = OBJ::LoadFromFile("assets/models/circle_in_3d.obj");
	m_texture = GFX->CreateTexture("assets/models/circle_in_3d.jpg");

	vertex_t star[] = {
		{ 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
	};

	m_starBuffer = GFX->CreateBuffer(BufferType::VERTEX_BUFFER);
	m_starBuffer->Orphan(sizeof(star), sizeof(vertex_t), star);

	m_starAttribs = GFX->CreateDrawAttribs(m_starBuffer);
	

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(-4.0f, 4.0f);

	CInstanceData data;
	
	for (float z = -8.0f; z < 0.0f; z += 0.3f) {
		for (float y = -3.0f; y < 3.0f; y += 0.3f) {
			for (float x = -8.0f; x < 8.0f; x += 0.3f) {
				S_CMatrix mat = GFX->CreateIdentityMatrix();
				mat->Translate(x + dist(mt), y + dist(mt), z + dist(mt));
				data.AddInstance(mat);
				m_starCount++;
			}
		}
	}

	data.LoadInto(m_starAttribs);

	S_CShader starShaders[] = {
		GFX->CreateShader(ShaderType::GFX_VERTEX_SHADER, "assets/shaders/point"),
		GFX->CreateShader(ShaderType::GFX_FRAGMENT_SHADER, "assets/shaders/point")
	};

	m_starProgram = GFX->CreateProgram(2, starShaders);

	float colour[] = {
		1.0f, 1.0f, 1.0f, 1.0f
	};

	m_starProgram->SetUniform(ShaderUniformType::GFX_VEC4F, "u_PointColour", colour);

	m_font.LoadFromFile("assets/fonts/Lato-Regular.ttf");
	m_dtText.InitText(&m_font, 16, "Hello World lol");
}

void CSceneBenchmark::OnResize(int width, int height) {
	m_mvpMatrix.projection = GFX->CreateIdentityMatrix();
	m_mvpMatrix.projection->Perspective(
		(float)M_PI / 2.0f,
		(float)width,
		(float)height,
		0.001f, 1000.0f
	);

	m_mvpMatrix.view = GFX->CreateIdentityMatrix();
	m_mvpMatrix.model = GFX->CreateIdentityMatrix();
}

void CSceneBenchmark::OnRender() {
	m_mvpMatrix.projection->LoadIdentity();
	m_mvpMatrix.projection->Perspective(
		(float)M_PI / 2.0f,
		(float)CGame::Inst->GetWidth(),
		(float)CGame::Inst->GetHeight(),
		0.001f, 1000.0f
	);

	m_mvpMatrix.model->LoadIdentity();
	m_mvpMatrix.model->Translate(0.0f, 0.0f, -2.0f);

	static float rotation = 0.0f;
	m_mvpMatrix.model->Rotate(0.0f, (float)M_PI / 180.0f * rotation, 0.0f);
	rotation += (float)CGame::Inst->GetDeltaTime() * 80.0f;

	GFX->SetDrawProgram(GFX->GetDefaultProgram());
	GFX->SetDrawTransform(m_mvpMatrix);
	GFX->SetDrawBuffer(m_buffer);
	GFX->SetDrawTexture(m_texture);

	static S_CDrawAttribs attribs;

	if (attribs == nullptr) {
		attribs = GFX->CreateDrawAttribs(m_buffer);
	}

	GFX->SetDrawAttributes(attribs);
	GFX->Draw(PrimitiveType::GFX_TRIANGLES);

	m_mvpMatrix.model->LoadIdentity();

	GFX->SetDrawProgram(m_starProgram);
	GFX->SetDrawTransform(m_mvpMatrix);
	GFX->SetDrawBuffer(m_starBuffer);
	GFX->SetDrawAttributes(m_starAttribs);
	GFX->SetDrawTexture(m_texture);
	
	GFX->DrawInstanced(PrimitiveType::GFX_POINTS, m_starCount);

	m_mvpMatrix.projection->LoadIdentity();
	m_mvpMatrix.projection->Orthographic(
		0.0f,
		(float)CGame::Inst->GetWidth(),
		(float)CGame::Inst->GetHeight(),
		0.0f
	);

	m_mvpMatrix.model->LoadIdentity();
	m_mvpMatrix.model->Translate(6.0f, 16.0f);

	std::ostringstream ss;
	ss << CGame::Inst->GetDeltaTime();

	m_dtText.SetText(ss.str());
	m_dtText.Render(m_mvpMatrix);
}
