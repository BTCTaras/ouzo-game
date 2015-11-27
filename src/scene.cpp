#include "scene.hpp"

#include "graphics.hpp"
#include "game.hpp"

void CScene::OnInit() {

}

void CScene::OnRender() {

}

void CScene::OnUpdate() {

}

void CScene::OnLeave(CScene *scene) {

}

void CScene::OnResize(int newWidth, int newHeight) {
	GFX->SetViewport(0, 0, newWidth, newHeight);
}

void CScene::OnClick(unsigned int button, float x, float y) {

}
