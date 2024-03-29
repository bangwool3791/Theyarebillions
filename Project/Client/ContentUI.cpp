#include "pch.h"
#include "ContentUI.h"

#include "CImGuiMgr.h"

#include "TreeUI.h"
#include "InspectorUI.h"
#include "CEditorCam.h"
#include "CGameObjectEx.h"

#include "CEditor.h"
#include "CSaveLoadMgr.h"

#include <Engine\CRes.h>
#include <Engine\CSound.h>
#include <Engine/CResMgr.h>
#include <Engine\CDevice.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTransform.h>
#include <Engine/CCamera.h>
#include <Engine\CPathMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>

#include <Script\CMouseScript.h>

ContentUI::ContentUI()
	:UI("ContentUI")
	, m_Tree{}
	, m_bDragEvent{false}
{
	m_Tree = new TreeUI("##ContentTree");
	AddChild(m_Tree);

	m_Tree->ShowSeperator(false);
	m_Tree->SetDummyRoot(true);

	m_Tree->AddDynamic_LBtn_Selected(this, (FUNC_1)&ContentUI::SetResourceToInspector);
	m_Tree->AddDynamic_DragDrop_World(this, (FUNC_1)&ContentUI::SetDragObject);

	m_pLevelMouseObject = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"MouseObject");
	m_pLevelTerrain  = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"LevelTerrain");
	m_pLevelCamera = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"MainCamera");
}

ContentUI::~ContentUI()
{

}

void ContentUI::update()
{
	if (CResMgr::GetInst()->IsChanged())
	{
		ResetContent();
	}

	if (KEY_PRESSED(KEY::LCTRL) && KEY_PRESSED(KEY::S))
	{
		TreeNode* pSelectedNode = m_Tree->GetSelectedNode();

		if (nullptr != pSelectedNode)
		{
			if (pSelectedNode->GetData())
			{
				RES_TYPE type = ((CRes*)pSelectedNode->GetData())->GetResType();

				wstring wstrRelativePath;

				switch (type)
				{
				case RES_TYPE::PREFAB:
					break;
				case RES_TYPE::MESHDATA:
					break;
				case RES_TYPE::COMPUTE_SHADER:
					break;
				case RES_TYPE::MATERIAL:
				{
					Ptr<CMaterial> pRes = (CMaterial*)pSelectedNode->GetData();
					if (pRes->GetName().find(L".mtrl") != std::string::npos) 
					{
						pRes->Save(pRes->GetName());
					}
					else
					{
						wchar_t sz_data[255] = { L"material\\" };
						wstring temp = lstrcat(sz_data, pRes->GetKey().data());
						lstrcpy(sz_data, temp.data());
						wstrRelativePath = lstrcat(sz_data, L".mtrl");
						pRes->Save(wstrRelativePath);
					}
				}
				break;
				case RES_TYPE::MESH:
					break;
				case RES_TYPE::TEXTURE:
					break;
				case RES_TYPE::SOUND:
					break;
				case RES_TYPE::GRAPHICS_SHADER:
					break;
				case RES_TYPE::END:
					break;
				default:
					break;
				}
			}
		}
	}
	UI::update();
}

void ContentUI::render_update()
{
	static int iLayer = 0;
	static bool bActive = false;

	ImGui::InputInt("Layer Index", &iLayer);

	ImGui::NewLine();

	if (bActive)
	{
		if (ImGui::Button("Active", Vec2(100.f, 50.f)))
			bActive = false;
	}
	else
	{
		if (ImGui::Button("Unactive", Vec2(100.f, 50.f)))
			bActive = true;
	}

	if (ImGui::Button("Save", Vec2(100.f, 50.f)))
	{
		CSaveLoadMgr::GetInst()->SavePrefab(L"prefab\\prefab.dat");
	}

	ImGui::SameLine();

	if (ImGui::Button("Load", Vec2(100.f, 50.f)))
	{
		CSaveLoadMgr::GetInst()->LoadPrefab(L"prefab\\prefab.dat");
		m_pTargetPrefab = nullptr;
		ResetContent();
	}

	if (m_bDragEvent && nullptr != m_pTargetPrefab && bActive)
	{
		if (KEY_RELEASE(KEY::LBTN))
		{
			Ray ray{};

			Vec2 p = CKeyMgr::GetInst()->GetMousePos();
			Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
			//float fScale = m_pCameraObejct->Camera()->GetOrthographicScale();
			//vResolution = vResolution * fScale;

			p.x = (2.0f * p.x) / vResolution.x - 1.0f;
			p.y = 1.0f - (2.0f * p.y) / vResolution.y;

			XMVECTOR det; //Determinant, needed for matrix inverse function call
			Vector3 origin = Vector3(p.x, p.y, -1);
			Vector3 faraway = Vector3(p.x, p.y, 1);

			const Matrix& matView = m_pLevelCamera->Camera()->GetViewMat();
			const Matrix& matProj = m_pLevelCamera->Camera()->GetProjMat();
			XMMATRIX invViewProj = XMMatrixInverse(&det, matView * matProj);
			Vector3 rayorigin = XMVector3Transform(origin, invViewProj);
			Vector3 rayend = XMVector3Transform(faraway, invViewProj);
			Vector3 raydirection = rayend - rayorigin;
			raydirection.Normalize();
			ray.position = rayorigin;
			ray.direction = raydirection;

			Vec3 vMousePos{};
			Vec3 vCameraPos = m_pLevelCamera->Transform()->GetRelativePos();

			BoundingFrustum fr(m_pLevelCamera->Camera()->GetProjMat());

			if (m_pLevelTerrain->Terrain()->GetMesh()->GetPosition(ray, vMousePos))
			{
				Vec2 vRes = CDevice::GetInst()->GetRenderResolution();

				float fDelta = m_pLevelCamera->Camera()->GetFar() - m_pLevelCamera->Camera()->GetNear();
				BoundingBox box(vMousePos - vCameraPos, Vec3(vRes.x, vRes.y, fDelta));
				BoundingFrustum fr(m_pLevelCamera->Camera()->GetProjMat());

				if (fr.Contains(box))
				{
					CGameObject* pGameObject = m_pTargetPrefab->Instantiate();
					Instantiate(pGameObject, vMousePos, iLayer);
				}
			}
		}
	}
}

void ContentUI::ResetContent()
{
	m_Tree->Clear();

	TreeNode* pRootNode = m_Tree->AddItem(nullptr, "Content", 0);

	for (UINT i{}; i < (UINT)RES_TYPE::END; ++i)
	{
		TreeNode* pResNode = m_Tree->AddItem(pRootNode, ToString((RES_TYPE)i), 0, true);

		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource((RES_TYPE(i)));

		auto iter = mapRes.begin();

		for (; iter != mapRes.end(); ++iter)
		{
			m_Tree->AddItem(pResNode, string(iter->first.begin(), iter->first.end()), (DWORD_PTR)iter->second.Get());
		}
	}
}

void ContentUI::ReloadContent()
{
	// Content 폴더에 있는 모든 리소스들을 검사 및 로딩
	wstring strFolderPath = CPathMgr::GetInst()->GetContentPath();
	FindContentFileName(strFolderPath);

	for (size_t i{}; i < m_vecContentName.size(); ++i)
	{
		RES_TYPE resType = GetResTypeByExt(m_vecContentName[i]);

		if (RES_TYPE::END == resType)
			continue;

		switch (resType)
		{
		case RES_TYPE::PREFAB:
			CResMgr::GetInst()->Load<CPrefab>(m_vecContentName[i]);
			break;
		case RES_TYPE::MESHDATA:
			break;
		case RES_TYPE::COMPUTE_SHADER:
			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecContentName[i]);
			break;
		case RES_TYPE::MESH:
			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(m_vecContentName[i]);
			break;
		case RES_TYPE::SOUND:
			CResMgr::GetInst()->Load<CSound>(m_vecContentName[i]);
			break;
		case RES_TYPE::GRAPHICS_SHADER:
			break;
		case RES_TYPE::END:
			break;
		default:
			break;
		}
	}

	for (UINT i{}; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource((RES_TYPE)i);
		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.cbegin();

		for (iter; iter != mapRes.end(); ++iter)
		{
			if (iter->second->IsEngineRes())
			{
				continue;
			}

			wstring strRelativePath = iter->second->GetRelativePath();

			if (strRelativePath.empty())
			{
				int a = 0;
			}

			if (!std::filesystem::exists(strFolderPath + strRelativePath))
			{
				if (iter->second->GetRefCount() <= 1)
				{
					// 삭제 가능
					tEvent evn = {};
					evn.eType = EVENT_TYPE::DELETE_RES;
					evn.wParam = i;
					evn.lParam = (DWORD_PTR)(iter->second.Get());

					CEventMgr::GetInst()->AddEvent(evn);

					MessageBox(nullptr, L"원본 리소스 삭제됨", L"리소스 변경 확인", MB_OK);
				}
				else
				{
					// 해당 리소스를 참조하는 객체가 있음	
					MessageBox(nullptr, L"사용 중 인 리소스/n리소스 삭제 실패", L"리소스 변경 확인", MB_OK);
				}
			}
		}
	}
}

void ContentUI::SetDragObject(DWORD_PTR _res)
{
	// _res : 클릭한 노드
	TreeNode* pSelectedNode = (TreeNode*)_res;
	m_pTargetPrefab = (CPrefab*)pSelectedNode->GetData();
	m_bDragEvent = true;
}

void ContentUI::SetResourceToInspector(DWORD_PTR _res)
{
	// _res : 클릭한 노드
	TreeNode* pSelectedNode = (TreeNode*)_res;
	CRes* pRes = (CRes*)pSelectedNode->GetData();
	// InspectorUI 에 클릭된 Resouce 를 알려준다.
	InspectorUI* Inspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	Inspector->SetTargetResource(pRes);
}

void ContentUI::FindContentFileName(const wstring& _strFolderPath)
{
	// 모든 파일명을 알아낸다.
	wstring strFolderPath = _strFolderPath + L"*.*";

	HANDLE hFindHandle = nullptr;

	WIN32_FIND_DATA data{};

	hFindHandle = FindFirstFile(strFolderPath.c_str(), &data);

	assert(hFindHandle);

	while (FindNextFile(hFindHandle, &data))
	{
		if (FILE_ATTRIBUTE_DIRECTORY == data.dwFileAttributes && wcscmp(data.cFileName, L".."))
		{
			FindContentFileName(_strFolderPath + data.cFileName + L"\\");
		}
		else if (FILE_ATTRIBUTE_DIRECTORY == data.dwFileAttributes && !wcscmp(data.cFileName, L".."))
		{

		}
		else
		{
			wstring strRelative = GetRelativePath(CPathMgr::GetInst()->GetContentPath(), _strFolderPath + data.cFileName);
			m_vecContentName.push_back(strRelative);
		}
	}
	FindClose(hFindHandle);
}

RES_TYPE ContentUI::GetResTypeByExt(const wstring& _filename)
{
	wstring strExt = path(_filename.c_str()).extension();

	if (strExt == L".pref")
		return RES_TYPE::PREFAB;
	else if (strExt == L".mdat")
		return RES_TYPE::MESHDATA;
	else if (strExt == L".mtrl")
		return RES_TYPE::MATERIAL;
	else if (strExt == L".mesh")
		return RES_TYPE::MESH;
	else if (strExt == L".dds" || strExt == L".DDS" || strExt == L".tag" || strExt == L".TGA"
		|| strExt == L".png" || strExt == L".jpg" || strExt == L".JPG" || strExt == L".jegp" || strExt == L".JPEG"
		|| strExt == L".bmp" || strExt == L".BMP")
		return RES_TYPE::TEXTURE;
	else if (strExt == L".mp3" || strExt == L".MP3" || strExt == L".wav" || strExt == L".WAV" || strExt == L".ogg" || strExt == L".OGG")
		return RES_TYPE::SOUND;
	else
		return RES_TYPE::END;
}