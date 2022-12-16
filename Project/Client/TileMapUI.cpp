#include "pch.h"
#include "TileMapUI.h"

#include "CImGuiMgr.h"

#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>

#include <Engine\CGameObject.h>
#include <Engine\CTileMap.h>

#include <Engine\CTexture.h>
#include <Engine\CResMgr.h>

#include <Engine\CScript.h>
#include <Script\CTileScript.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "ProgressUI.h"

TileMapUI::TileMapUI()
	: UI("TileMap")
	, m_vTileSize{1.f, 1.f }
{
}

TileMapUI::~TileMapUI()
{
}

void TileMapUI::begin()
{
}

void TileMapUI::update()
{
	UI::update();
}

void TileMapUI::render_update()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.8f, 0.8f));
	ImGui::Button("Tile");
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	ImGui::SameLine();

	if (ImGui::Button("Apply"))
	{
		ProgressUI* pProgressUI = dynamic_cast<ProgressUI*>(CImGuiMgr::GetInst()->FindUI("ProgressUI"));
		assert(pProgressUI);
		m_pEditTileMap->EidtApply();
		pProgressUI->Open();
	}

	ImGui::InputFloat("tile size x", &m_vTileSize.x, 0.01f, 1.0f, "%.3f");
	ImGui::InputFloat("tile size y", &m_vTileSize.y, 0.01f, 1.0f, "%.3f");

	m_pEditTileMap->SetTileSizeX(m_vTileSize.x);
	m_pEditTileMap->SetTileSizeY(m_vTileSize.y);

	for (UINT i{}; i < TEX_32; ++i)
	{
		wstring str = L"Tile";
		str += std::to_wstring(i);
		ImTextureID myImage = CResMgr::GetInst()->FindRes<CTexture>(str)->GetSRV().Get();

		if (ImGui::ImageButton(myImage, ImVec2(50.f, 50.f)))
		{
			m_pEditTileObject->GetScript<CTileScript>(L"CTileScript")->SetTileInfo(i);
		}

		if (i == 0 || (i % 7 != 0))
		{
			ImGui::SameLine();
		}
	}
}

void TileMapUI::Initialize(void* pAddr)
{
	m_pEditTileObject = (CGameObject*)pAddr;
	m_pEditTileMap = static_cast<CTileMap*>(m_pEditTileObject->GetRenderComponent());
}




