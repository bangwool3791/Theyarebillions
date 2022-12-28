#include "pch.h"
#include "ContentUI.h"

#include "CImGuiMgr.h"

#include "TreeUI.h"
#include "InspectorUI.h"
#include "CEditorCam.h"
#include "CGameObjectEx.h"

#include "CEditor.h"

#include <Engine\CRes.h>
#include <Engine/CResMgr.h>
#include <Engine\CDevice.h>
#include <Engine\CKeyMgr.h>
#include <Engine\CTransform.h>
#include <Engine/CCamera.h>
#include <Engine\CPathMgr.h>

ContentUI::ContentUI()
	:UI("Content")
	, m_Tree{}
	, m_bDragEvent{false}
{
	m_Tree = new TreeUI("##ContentTree");
	AddChild(m_Tree);

	m_Tree->ShowSeperator(false);
	m_Tree->SetDummyRoot(true);

	m_Tree->AddDynamic_LBtn_Selected(this, (FUNC_1)&ContentUI::SetResourceToInspector);
	m_Tree->AddDynamic_DragDrop_World(this, (FUNC_1)&ContentUI::SetDragObject);
	ResetContent();
}

ContentUI::~ContentUI()
{

}

void ContentUI::update()
{
	static bool bChk = false;

	if (bChk == false)
	{
		ReloadContent();
		bChk = true;
	}
	if (CResMgr::GetInst()->IsChanged())
	{
		ResetContent();
	}

	if (m_bDragEvent && nullptr != m_pTargetPrefab)
	{
		if (KEY_RELEASE(KEY::LBTN))
		{
			CGameObjectEx* pCameraObject = CEditor::GetInst()->FindByName(L"Editor Camera");
			Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

			Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();
			float fScale = pCameraObject->Camera()->GetOrthographicScale();
			Resolution *= fScale;
			Vec3  vCameraPos = pCameraObject->Transform()->GetRelativePos();

			Vec3 vPos{ vMousePos.x, vMousePos.y, 1.f };
			vPos.x -= Resolution.x * 0.5f;
			vPos.y = vPos.y * -1.f + Resolution.y * 0.5f;
			vPos *= fScale;
			vPos.x += vCameraPos.x;
			vPos.y += vCameraPos.y;

			if (Resolution.x * -0.5f <= vPos.x && Resolution.y * -0.5f <= vPos.y &&
				Resolution.x * 0.5f >= vPos.x && Resolution.y * 0.5f >= vPos.y)
			{
				CGameObject* pGameObject = m_pTargetPrefab->Instantiate();

				Instantiate(pGameObject, vPos, 1);
			}
		}
	}

	if (KEY_PRESSED(KEY::LCTRL) && KEY_PRESSED(KEY::S))
	{
		TreeNode* pSelectedNode = m_Tree->GetSelectedNode();

		if (nullptr != pSelectedNode)
		{
			if (pSelectedNode->GetData())
			{
				RES_TYPE type = ((CRes*)pSelectedNode->GetData())->GetResType();

				wstring strRelativePath;

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
					strRelativePath = L"\\material\\";
					strRelativePath += pRes->GetKey();
					pRes->Save(strRelativePath);
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
				MessageBox(nullptr, L"ReloadContent File Path", L"Error", MB_OK);
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

	if (INVALID_HANDLE_VALUE == hFindHandle)
	{
		MessageBox(nullptr, L"FindFirstFile", L"Error", MB_OK);
		return;
	}

	while (FindNextFile(hFindHandle, &data))
	{
		if (FILE_ATTRIBUTE_DIRECTORY == data.dwFileAttributes && wcscmp(data.cFileName, L".."))
		{
			FindContentFileName(_strFolderPath + data.cFileName + L"\\");
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