#include "pch.h"
#include "MenuUI.h"
#include "InspectorUI.h"

#include "CImGuiMgr.h"

#include <Engine\CGameObject.h>
#include <Engine\CEventMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CScript.h>
#include <Script\CScriptMgr.h>

MenuUI::MenuUI()
	:UI("##MenuUI")
{

}
MenuUI::~MenuUI()
{

}

void MenuUI::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z"))
            {

            }
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Component"))
        {
            if (ImGui::BeginMenu("AddScript"))
            {
                
                vector<string> vecScriptName{};
                CScriptMgr::GetScriptInfo(vecScriptName);

                for (size_t i{}; i < vecScriptName.size(); ++i)
                {
                    if (ImGui::MenuItem(vecScriptName[i].c_str()))
                    {
                        InspectorUI* pInspector = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
                        CGameObject* pTargetObject = pInspector->GetTargetObject();

                        if (pTargetObject)
                        {
                            pTargetObject->AddComponent(CScriptMgr::GetScript(vecScriptName[i]));
                            //���� �뵵
                            pInspector->SetTargetObject(pTargetObject);
                        }
                    }
                }
                ImGui::EndMenu();
            }



            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Level"))
        {
            LEVEL_STATE State = CLevelMgr::GetInst()->GetLevelState();

            bool PlayEnable = true;
            bool PauseEnable = true;
            bool StopEnable = true;

            if (LEVEL_STATE::PLAY == State)
                PlayEnable = false;
            else
                PlayEnable = true;

            if (LEVEL_STATE::PLAY != State)
                PauseEnable = false;
            else
                PauseEnable = true;

            if (LEVEL_STATE::STOP == State)
                StopEnable = false;
            else
                StopEnable = true;

            if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
            {
                tEvent evn = {};
                evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
                evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
                CEventMgr::GetInst()->AddEvent(evn);
            }

            if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
            {
                tEvent evn = {};
                evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
                evn.wParam = (DWORD_PTR)LEVEL_STATE::PAUSE;
                CEventMgr::GetInst()->AddEvent(evn);
            }

            if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
            {
                tEvent evn = {};
                evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
                evn.wParam = (DWORD_PTR)LEVEL_STATE::STOP;
                CEventMgr::GetInst()->AddEvent(evn);
            }


            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::render_update()
{

}