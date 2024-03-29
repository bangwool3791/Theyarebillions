#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;
class ScriptUI;

#include <Engine\Ptr.h>
#include <Engine\CRes.h>
class ResUI;

class InspectorUI :
    public UI
{
private:
    vector<ScriptUI*>   m_vecScriptUI;

    CGameObject*        m_TargetObj;
    ComponentUI*        m_arrComUI[(UINT)COMPONENT_TYPE::END];
    ComponentUI*        m_arrObjUI[(UINT)OBJECT_TYPE::END];

    Ptr<CRes>           m_TargetRes;
    ResUI*              m_arrResUI[(UINT)RES_TYPE::END];
public:

    void SetTargetObject(CGameObject* _Target);
    void SetTargetResource(CRes* _Resource);
    void SetLight(CGameObject* _Light);
    void InitializeScriptUI();
    CGameObject* GetTargetObject() { return m_TargetObj; }
private:
    virtual void update() override;
    virtual void render_update() override;

public:
    InspectorUI();
    ~InspectorUI();
};

