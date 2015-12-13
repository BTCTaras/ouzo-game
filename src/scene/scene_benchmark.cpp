#include "scene/scene_benchmark.hpp"

#include "graphics_common/wavefront.hpp"
#include "graphics_common/graphics.hpp"
#include "graphics_common/font.hpp"
#include "audio/sound.hpp"
#include "audio/sound_manager.hpp"
#include "game.hpp"

#include <random>
#include <sstream>
#include <math.h>

CSceneBenchmark::CSceneBenchmark()
	:	m_starCount(0),
		m_hatCount(0)
{
}

void CSceneBenchmark::OnInit() {
	m_buffer = OBJ::LoadFromFile("assets/models/circle_in_3d.obj");
	m_hatBuffer = OBJ::LoadFromFile("assets/models/hat.obj");

	m_texture = GFX->CreateTexture("assets/models/circle_in_3d.jpg");
	m_hatTexture = GFX->CreateTexture("assets/models/hat.jpg");

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

	for (float z = -8.0f; z < -5.0f; z += 0.5f) {
		for (float y = -6.0f; y < 6.0f; y += 0.5f) {
			for (float x = -12.0f; x < 12.0f; x += 0.5f) {
				S_CMatrix mat = GFX->CreateIdentityMatrix();
				mat->Translate(x + dist(mt), y + dist(mt), z);
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

	m_hatAttribs = GFX->CreateDrawAttribs(m_hatBuffer);

	std::uniform_real_distribution<float> angleDist(0, 2 * M_PI);
	std::uniform_real_distribution<float> scaleDist(0.01f, 0.1f);

	CInstanceData hatData;

	for (float d = 1.5f; d < 4.0f; d += 0.1f) {
		for (float angle = 0.0f; angle < 360.0f; angle += 11.1125f) {
			float rad = (float)(M_PI / 180.0f * angle);

			S_CMatrix mat = GFX->CreateIdentityMatrix();
			float x = cosf(rad + angleDist(mt)) * d;
			float y = sinf(rad + angleDist(mt)) * d;

			mat->Translate(x, 0.0f, -y - 4.0f);
			mat->Rotate(angleDist(mt), angleDist(mt), angleDist(mt));

			float scale = scaleDist(mt);
			mat->Scale(scale, scale, scale);
			hatData.AddInstance(mat);

			m_hatCount++;
		}
	}

	hatData.LoadInto(m_hatAttribs);

	std::ostringstream ss;
	ss << "Stars: " << m_starCount << "; Hats: " << m_hatCount;
	m_dtText.InitText(&m_font, 32, ss.str());

	SFX->PlayMusic("assets/sound/bg.mp3");
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

	m_mvpMatrix.view->LoadIdentity();
	m_mvpMatrix.view->Translate(0.0f, -0.5f, 0.0f);
	m_mvpMatrix.view->Rotate((float)(M_PI / 180.0f * 10.0f), 0.0f, 0.0f);

	m_mvpMatrix.model->LoadIdentity();
	m_mvpMatrix.model->Translate(0.0f, 0.0f, -4.0f);

	static float rotation = 0.0f;
	m_mvpMatrix.model->Rotate(0.0f, (float)M_PI / 180.0f * -rotation, 0.0f);
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
	m_mvpMatrix.model->Rotate(0.0f, (float)M_PI / 180.0f * rotation, 0.0f);

	GFX->SetDrawTransform(m_mvpMatrix);
	GFX->SetDrawBuffer(m_hatBuffer);
	GFX->SetDrawAttributes(m_hatAttribs);
	GFX->SetDrawTexture(m_hatTexture);

	GFX->DrawInstanced(PrimitiveType::GFX_TRIANGLES, m_hatCount);

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

	m_mvpMatrix.view->LoadIdentity();
	m_mvpMatrix.model->LoadIdentity();
	m_mvpMatrix.model->Translate(6.0f, 32.0f);

	m_dtText.Render(m_mvpMatrix);
}
