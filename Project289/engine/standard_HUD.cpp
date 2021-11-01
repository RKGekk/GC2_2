#include "standard_HUD.h"
#include "engine.h"
#include "d3d_renderer11.h"
#include "../actors/character_stats_component.h"
#include "../tools/memory_utility.h"
#include "../actors/memo_component.h"

StandardHUD::StandardHUD(ProcessManager* pm) : m_pm(pm) {
	D3DRenderer11* renderer = static_cast<D3DRenderer11*>(g_pApp->GetRenderer());
	m_sprite_batch = std::make_unique<DirectX::SpriteBatch>(renderer->GetDeviceContext());
	m_sprite_font = std::make_unique<DirectX::SpriteFont>(renderer->GetDevice(), L"data/fonts/mefont.spritefont");
	m_fps_timer.Start();
}

StandardHUD::~StandardHUD() {}

HRESULT StandardHUD::VOnRestore() {
	return S_OK;
}

HRESULT StandardHUD::VOnRender(double fTime, float fElapsedTime) {
	m_fps_timer.Tick();

	static int fpsCounter = 0;
	static std::wstring fpsString = L"FPS: 0";
	fpsCounter += 1;
	if (m_fps_timer.TotalTime() > 1.0f) {
		fpsString = L"FPS: " + std::to_wstring(fpsCounter);
		fpsCounter = 0;
		m_fps_timer.Reset();
	}

	StrongActorPtr pActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName("pers"));
	std::shared_ptr<CharacterStatsComponent> pCharacterStatsComponent;
	if (pActor) {
		pCharacterStatsComponent = MakeStrongPtr(pActor->GetComponent<CharacterStatsComponent>(CharacterStatsComponent::g_Name));
	}
	std::wstring dropsString = L"Drops: ";
	if (pCharacterStatsComponent) {
		dropsString += std::to_wstring(pCharacterStatsComponent->GetDrops());
	}


	StrongActorPtr pMemoActor = MakeStrongPtr(g_pApp->GetGameLogic()->VGetActorByName("memo2"));
	std::shared_ptr<MemoComponent> pMemoComponent;
	if (pMemoActor) {
		pMemoComponent = MakeStrongPtr(pMemoActor->GetComponent<MemoComponent>(MemoComponent::g_Name));
	}
	std::wstring timerString = L"Remaining time: ";
	if (pMemoComponent) {
		timerString += std::to_wstring(((int)pMemoComponent->GetStartTime()) - ((int)fTime));
	}
	else {
		timerString += std::to_wstring(600 - ((int)fTime));
	}


	m_sprite_batch->Begin();

	DirectX::XMVECTOR fps_pos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR fps_color = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR fps_origin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR fps_scale = DirectX::XMVectorSet(0.3f, 0.3f, 0.3f, 1.0f);
	m_sprite_font->DrawString(m_sprite_batch.get(), fpsString.c_str(), fps_pos, DirectX::Colors::White, 0.0f, fps_origin, fps_scale);

	const EngineOptions& options = g_pApp->GetConfig();
	float screen_width = options.m_screenWidth;
	DirectX::XMVECTOR timer_pos = DirectX::XMVectorSet(screen_width * 0.5f + screen_width * 0.25f - screen_width * 0.125f + screen_width * 0.0625f, 0.0f, 10.0f, 0.0f);
	DirectX::XMVECTOR timer_color = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR timer_origin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR timer_scale = DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
	m_sprite_font->DrawString(m_sprite_batch.get(), timerString.c_str(), timer_pos, DirectX::Colors::White, 0.0f, timer_origin, timer_scale);

	DirectX::XMVECTOR drop_pos = DirectX::XMVectorSet(screen_width * 0.5f, 0.0f, 10.0f, 0.0f);
	DirectX::XMVECTOR drop_color = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR drop_origin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR drop_scale = DirectX::XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
	m_sprite_font->DrawString(m_sprite_batch.get(), dropsString.c_str(), drop_pos, DirectX::Colors::White, 0.0f, drop_origin, drop_scale);

	m_sprite_batch->End();

	return S_OK;
}

void StandardHUD::VOnUpdate(float deltaMilliseconds) {}

int StandardHUD::VGetZOrder() const {
	return 3;
}

LRESULT StandardHUD::VOnMsgProc(HWND m_hWnd, UINT m_uMsg, WPARAM m_wParam, LPARAM m_lParam) {
	return 0;
}
