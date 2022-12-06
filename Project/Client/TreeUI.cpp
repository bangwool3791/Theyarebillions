#include "pch.h"
#include "TreeUI.h"
#include <Engine\CComponent.h>
#include "CGameObjectEx.h"
#include "CEditor.h"
// ========
// TreeNode
// ========

TreeNode::TreeNode()
	:m_strName{}
	, m_data{}
	, m_vecChildNode{}
	, m_ParentNode{}
	, m_TreeUI{}
	, m_iIdx{}
	, m_bFrame{}
	, m_bSelected{}
{
	
}

TreeNode::~TreeNode()
{
	for (auto iter{ m_vecChildNode.begin() }; iter != m_vecChildNode.end(); ++iter)
	{
		if ((*iter)->m_pGameObject)
		{
			Safe_Delete((*iter)->m_pGameObject);
		}

		Safe_Delete(*iter);
	}
}

void TreeNode::render_update()
{
	int iFlag = 0;
	//���� ǥ�� ����ΰ�
	if (m_bFrame)
		iFlag |= ImGuiTreeNodeFlags_Framed;
	//���� �Ǿ��°�
	if (m_bSelected)
		iFlag |= ImGuiTreeNodeFlags_Selected;
	//Leaf ����ΰ�
	if (m_vecChildNode.empty())
		iFlag |= ImGuiTreeNodeFlags_Leaf;


	string strName = m_strName;
	//���� ǥ�� ����̸鼭 Leaf����̸�
	//��ĭ Shift
	if (m_bFrame && m_vecChildNode.empty())
		strName = "\t" + strName;

	// �ڿ� �ĺ���ȣ �ٿ��� �̸�(Ű) �� ���� �ʵ��� ��
	char szTag[50] = "";
	sprintf_s(szTag, 50, "##%d", m_iIdx);
	strName += szTag;

	//Ʈ�� UI Render
	if (ImGui::TreeNodeEx(strName.c_str(), iFlag))
	{
		// �巡�� üũ
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			/*
			* Ʈ�� �巡�� ���� ��� -> ������
			* m_DragDropFunc �Լ� ������ ���, �ڽ� ��� �Է� �Ķ���� ����
			*/
			m_TreeUI->SetBeginDragNode(this);
			//Tree�� Tree Node ������ ����
			ImGui::SetDragDropPayload(m_TreeUI->GetName().c_str(), (void*)this, sizeof(TreeNode));
			//�巡�� �� ������ �̸� ���
			std::string delimiter = "##";
			std::string token = strName.substr(0, strName.find(delimiter)); 
			ImGui::Text(token.c_str());

			//�巡�� �̺�Ʈ ��
			ImGui::EndDragDropSource();
		}
		// Ŭ�� üũ
		else if (!m_bFrame && ImGui::IsItemHovered(0) && ImGui::IsMouseClicked(0))
		{
			/*
			* ���� ���� ���
			* ��� �ȿ� �ִ� Resource �����͸� �ν����ͷ� �����Ѵ�.
			*/
			m_TreeUI->SetSelectedNode(this);
		}

		// ��� üũ(��� ���)
		if (ImGui::BeginDragDropTarget())
		{
			/*
			* m_DragDropFunc �Լ� ������ ���, �θ� ��� �Է� �Ķ���� ����
			*/
			m_TreeUI->SetDropTargetNode(this);

			ImGui::EndDragDropTarget();
		}

		/*
		* TreeNode ���� �ڽ� ��� render
		*/
		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}
}

// ======
// TreeUI
// ======
UINT TreeUI::m_iNextNodeIdx = 0;

TreeUI::TreeUI(const string& _strName)
	: UI(_strName)
	, m_RootNode(nullptr)
	, m_bDummyRootUse(false)
	, m_SelectedNode(nullptr)
	, m_BeginDragNode(nullptr)
	, m_DropTargetNode(nullptr)
	, m_SelectInst(nullptr)
	, m_SelectFunc(nullptr)
	, m_DragDropInst(nullptr)
	, m_DragDropFunc(nullptr)
{
}

TreeUI::~TreeUI()
{
	Clear();
}

void TreeUI::update()
{

}

void TreeUI::render_update()
{
	if (nullptr == m_RootNode)
		return;

	if (!m_bDummyRootUse)
	{
		m_RootNode->render_update();
	}
	else
	{
		const vector<TreeNode*>& vecChildNode = m_RootNode->GetChild();

		for (size_t i{}; i < vecChildNode.size(); ++i)
		{
			vecChildNode[i]->render_update();
		}
	}

	if (ImGui::IsMouseReleased(0))
	{
		m_BeginDragNode = nullptr;
		m_DropTargetNode = nullptr;
	}
}

TreeNode* TreeUI::AddItem(TreeNode* _parent, const string& _strName, DWORD_PTR _data, bool _IsFrame)
{
	TreeNode* pNode = new TreeNode;
	pNode->SetNodeName(_strName);
	pNode->SetData(_data);
	pNode->SetFrame(_IsFrame);
	pNode->m_TreeUI = this;
	pNode->m_iIdx = m_iNextNodeIdx++;

	if (nullptr == _parent)
	{
		//�̹� ��Ʈ�� ���� �� ��尡 �ִٸ� assert
		assert(!m_RootNode);
		m_RootNode = pNode;
	}
	else
	{
		_parent->AddChild(pNode);
	}

	return pNode;
}

TreeNode* TreeUI::AddObjectEx(TreeNode* _parent, const string& _strName, CGameObjectEx* _pObj, bool _IsFrame)
{
	TreeNode* pNode = new TreeNode;
	pNode->SetNodeName(_strName);
	pNode->SetGameObject(_pObj);
	pNode->SetFrame(_IsFrame);
	pNode->m_TreeUI = this;
	pNode->m_iIdx = m_iNextNodeIdx++;

	if (nullptr == _parent)
	{
		//�̹� ��Ʈ�� ���� �� ��尡 �ִٸ� assert
		assert(!m_RootNode);
		m_RootNode = pNode;
	}
	else
	{
		_parent->AddChild(pNode);
	}

	return pNode;
}

TreeNode* TreeUI::GetNode(CGameObjectEx* _pObj)
{
	if ((CGameObjectEx*)m_RootNode->GetData() == _pObj)
	{
		return m_RootNode;
	}
	
	vector<TreeNode*>::iterator iter = m_RootNode->m_vecChildNode.begin();

	for (; iter != m_RootNode->m_vecChildNode.end(); ++iter)
	{
		if ((CGameObjectEx*)(*iter)->GetData() == _pObj)
		{
			return (*iter);
		}
	}

	return nullptr;
}

void TreeUI::Clear()
{
	if (nullptr != m_RootNode)
	{
		delete m_RootNode;
		m_RootNode = nullptr;
	}
}

void TreeUI::SetSelectedNode(TreeNode* _SelectedNode)
{
	if (nullptr != m_SelectedNode)
	{
		m_SelectedNode->m_bSelected = false;
	}

	m_SelectedNode = _SelectedNode;
	m_SelectedNode->m_bSelected = true;

	/*
	* Content UI, Outliner UI -> Tree UI, has a ���� ���
	*						  -> ���� m_SelectInst, m_SelectFunc �ʱ�ȭ
	*			 
	* Content, Outliner UI-> SetObjectToInspector �Լ� ȣ��
	*/
	if (m_SelectInst && m_SelectFunc)
	{
		(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_SelectedNode);
	}
}

/*
* Tree Drop
* Node, has a ���� TreeNode->SetDropTargetNode
*/
#include <Engine\CGameObject.h>

void TreeUI::SetDropTargetNode(TreeNode* _DropTargetNode)
{
	m_DropTargetNode = _DropTargetNode;

	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetName().c_str()))
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_BeginDragNode, (DWORD_PTR)m_DropTargetNode);
		}
	}
	/*
	* DummyTree -> ObjectTree
	* ObjectTree 
	*/
	else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("##DummyTree"))
	{
		if (GetName() == "##ModelTree")
		{
			TreeNode* pNode = (TreeNode*)payload->Data;
			CGameObjectEx* pGameObject = (CGameObjectEx*)pNode->GetData();
			wstring strName = pGameObject->GetName().c_str();
			CGameObjectEx* pTarget = pGameObject->Clone();
			AddItem(m_RootNode, string(strName.begin(), strName.end()), (DWORD_PTR)pTarget);
			CEditor::GetInst()->AddEditObject(pTarget);
		}
	}
	else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("##ModelComTree"))
	{
		/*
		* Outliner UI �ܰ�, GameObject Resource �߰�.
		*/
		CGameObjectEx* pGameObjectEx = (CGameObjectEx*)m_DropTargetNode->GetData();

		if (pGameObjectEx)
		{
			TreeNode* pNode = (TreeNode*)payload->Data;
			CComponent* pCom = (CComponent*)pNode->GetData();
			pGameObjectEx->AddComponent(pCom->Clone());

			pNode = GetNode(pGameObjectEx);
			AddItem(pNode, string(pCom->GetName().begin(), pCom->GetName().end()), (DWORD_PTR)pCom);
		}
	}
}