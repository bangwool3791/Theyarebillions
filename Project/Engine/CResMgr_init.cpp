#include "pch.h"
#include "CResMgr.h"

#include "CTransform.h"

#include "CTexture.h"
#include "CMeshRender.h"

#include "CPaintShader.h"

void CResMgr::init()
{
	CreateDefaultMesh();

	CreateDefaultTexture();

	CreateDefaultGraphicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();

}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture;

	pTex->Create(_iWidth, _iHeight, _eFormat, _iBindFlag);

	AddRes<CTexture>(_strKey, pTex.Get());
	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture;

	pTex->Create(_Tex2D);

	AddRes<CTexture>(_strKey, pTex.Get());
	return pTex;
}

void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;

	back_insert_iterator iterVtx{ vecVtx };
	back_insert_iterator iterIdx{ vecIdx };
	Vtx v;
	CMesh* pMesh = nullptr;

	// 사각형 메쉬 만들기
	v.vPos = Vec3(-0.5f, 0.5f, 1.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, 0.5f, 1.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	*iterVtx = v;

	v.vPos = Vec3(0.5f, -0.5f, 1.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	*iterVtx = v;

	v.vPos = Vec3(-0.5f, -0.5f, 1.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);
	vecIdx.clear();

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;
	*iterIdx = 3;
	*iterIdx = 0;

	pMesh = new CMesh;
	pMesh->SetName(L"RectMesh_Debug");
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	// 원형메쉬 만들기
	// 중심점	
	v.vPos = Vec3(0.f, 0.f, 1.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	*iterVtx = v;

	int iSlice = 40;
	float fRadius = 0.5f;
	float fTheta = XM_2PI / (float)iSlice;

	for (int i = 0; i < iSlice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 1.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		*iterVtx = v;
	}

	for (UINT i = 0; i < (UINT)iSlice; ++i)
	{
		*iterIdx = 0;
		if (i == iSlice - 1)
		{
			*iterIdx = 1;
		}
		else
		{
			*iterIdx = i + 2;
		}
		*iterIdx = i + 1;
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh);
	vecIdx.clear();

	for (UINT i = 0; i < (UINT)iSlice; ++i)
	{
		*iterIdx = i + 1;
	}
	*iterIdx = 1;
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	v.vPos = Vec3(-0.5f, 0.f, 1.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.5f);
	*iterVtx = v;
	v.vPos = Vec3(0.f, 0.5f, 1.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.f);
	*iterVtx = v;
	v.vPos = Vec3(0.5f, 0.f, 1.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.5f);
	*iterVtx = v;
	v.vPos = Vec3(0.f, -0.5f, 1.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 1.f);
	*iterVtx = v;

	*iterIdx = 0;
	*iterIdx = 1;
	*iterIdx = 2;

	*iterIdx = 0;
	*iterIdx = 2;
	*iterIdx = 3;

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"Tile", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	/*정점 메쉬
	*/
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT idx = 0;

	pMesh = new CMesh;
	pMesh->Create(&v, 1, &idx, 1);
	AddRes<CMesh>(L"PointMesh", pMesh);
}

void CResMgr::CreateDefaultTexture()
{
	Load<CTexture>(L"Plane", L"texture\\Player.bmp");
	Load<CTexture>(L"Smoke", L"texture\\smokeparticle.png");
	Load<CTexture>(L"Character", L"texture\\Character.png");
	Load<CTexture>(L"Link", L"texture\\link.png");

	Load<CTexture>(L"DeadCellColor", L"texture\\beheaded.png");
	Load<CTexture>(L"DeadCellNormal", L"texture\\beheaded_n.png");

	Load<CTexture>(L"SmokeParticle", L"texture\\particle\\smokeparticle.png");
	Load<CTexture>(L"CartoonSmoke", L"texture\\particle\\CartoonSmoke.png");
	Load<CTexture>(L"Bubbles50px", L"texture\\particle\\Bubbles50px.png");
	// NoiseTexture
	Load<CTexture>(L"Noise_01", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"Noise_02", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"Noise_03", L"texture\\noise\\noise_03.jpg");

	Load<CTexture>(L"Sparks", L"texture\\particle\\Sparks.png");
	Load<CTexture>(L"TileTex", L"texture\\TILE.bmp");

	for (UINT i{}; i < 32; i++)
	{
		wstring name = L"Tile" + std::to_wstring(i);
		wstring str = L"texture\\Terrain\\Tile\\Tile";
		str += std::to_wstring(i) + L".png";
		Load<CTexture>(name, str);
	}
	//Load<CTexture>(L"Tile0", L"texture\\Terrain\\Tile\\Tile0.png");
	//Load<CTexture>(L"Tile0", L"texture\\Terrain\\Tile\\Tile0.png");
	//Load<CTexture>(L"Tile0", L"texture\\Terrain\\Tile\\Tile0.png");
	//Load<CTexture>(L"Tile0", L"texture\\Terrain\\Tile\\Tile0.png");
	//Load<CTexture>(L"Tile31", L"texture\\Terrain\\Tile\\Tile31.png");
	CreateTexture(L"UAVTex", 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE |
		D3D11_BIND_UNORDERED_ACCESS);
}

void CResMgr::CreateDefaultGraphicsShader()
{
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "COLOR");
	AddInputLayout(DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD");

	CGraphicsShader* pShader = nullptr;

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_DeadCell");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_DeadCell");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	AddRes<CGraphicsShader>(L"TestShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam(INT_0, "Test Int");
	pShader->AddScalarParam(FLOAT_2, "Test Float");
	pShader->AddScalarParam(VEC2_3, "Test Vec2");
	pShader->AddScalarParam(VEC4_2, "Test Vec4");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");
	/*
	* 앞에는 그려지나, 언제던지 먼 사물체 그려질 수 있다.
	*/
	AddRes<CGraphicsShader>(L"Std2DShader", pShader);

	/*
	* 알파블랜드용 셰이더
	*/
	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"Std2DAlphaBlendShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\DragEffect.fx", "VS_DragEffect");
	pShader->CreatePixelShader(L"shader\\DragEffect.fx", "PS_DragEffect");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"DragEffectShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\editor.fx", "VS_Grid");
	pShader->CreatePixelShader(L"shader\\editor.fx", "PS_Grid");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"EditorShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\debugdraw.fx", "VS_DebugDraw");
	pShader->CreatePixelShader(L"shader\\debugdraw.fx", "PS_DebugDraw");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"DebugDrawShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\selectedunit.fx", "VS_DebugDraw");
	pShader->CreatePixelShader(L"shader\\selectedunit.fx", "PS_DebugDraw");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	AddRes<CGraphicsShader>(L"UnitSelectUIShader", pShader);

	pShader = new CGraphicsShader();
	pShader->CreateVertexShader(L"shader\\tile.fx", "VS_Tile");
	pShader->CreatePixelShader(L"shader\\tile.fx", "PS_Tile");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	AddRes<CGraphicsShader>(L"TileShader", pShader);

	// ParticleRenderShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\particlerender.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particlerender.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particlerender.fx", "PS_ParticleRender");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"ParticleRenderShader", pShader);

	// Instancing Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\objectrenderer.fx", "VS_ObjectRender");
	pShader->CreatePixelShader(L"shader\\objectrenderer.fx", "PS_ObjectRender");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes<CGraphicsShader>(L"ObjectRenderShader", pShader);

	// Instancing Shadow Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\shadowrenderer.fx", "VS_ShadowRender");
	pShader->CreatePixelShader(L"shader\\shadowrenderer.fx", "PS_ShadowRender");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"ShadowRenderShader", pShader);

	// PostProcess Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_PostProcess");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_PostProcess");

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);

	AddRes<CGraphicsShader>(L"PostProcessShader", pShader);

	// TileMap Shader
	pShader = new CGraphicsShader;
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreateGeometryShader(L"shader\\tilemap.fx", "GS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");
	/*
	* 투명한 타일이 있을 경우
	*/
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	/*
	* 알파블랜드는 타일은 생각하기 힘들다
	*/
	AddRes<CGraphicsShader>(L"TileMapShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\ref_animation.fx", "VS_RefAni");
	pShader->CreatePixelShader(L"shader\\ref_animation.fx", "PS_RefAni");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddRes<CGraphicsShader>(L"RefAniShader", pShader);
}

#include "CComputeShader.h"
#include "CParticleUpdateShader.h"

void CResMgr::CreateDefaultComputeShader()
{
	CComputeShader* pShader = nullptr;
	pShader = new CPaintShader;
	pShader->CreateComputeShader(L"shader\\compute.fx", "CS_Paint");
	AddRes<CComputeShader>(L"PaintShader", pShader);

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pShader);

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\particleupdatewood.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateWoodShader", pShader);
}

void CResMgr::CreateDefaultMaterial()
{

	CMaterial* pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"TestShader"));
	AddRes(L"TestMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"Std2DMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"Std2DAlphaBlendShader"));
	AddRes(L"Std2DAlphaBlendMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"DragEffectShader"));
	AddRes(L"MouseDragMaterial", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"EditorShader"));
	AddRes(L"EditMaterial", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"UnitSelectUIShader"));
	AddRes(L"UnitSelectUIMaterial", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"TileShader"));
	AddRes(L"TileMaterial", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes(L"ParticleRenderMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes(L"ParticleRenderWoodMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"ObjectRenderShader"));
	AddRes(L"ObjectMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"PostProcessShader"));
	AddRes<CMaterial>(L"PostProcessMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"DebugDrawShader"));
	AddRes<CMaterial>(L"DebugDrawMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"ShadowRenderShader"));
	AddRes<CMaterial>(L"ShadowMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes<CMaterial>(L"TileMapMtrl", pMaterial);

	pMaterial = new CMaterial(true);
	pMaterial->SetShader(FindRes<CGraphicsShader>(L"RefAniShader"));
	AddRes<CMaterial>(L"RefAniMtrl", pMaterial);
}

int GetSizeofFormat(DXGI_FORMAT _eFormat)
{
	int iRetByte = 0;
	switch (_eFormat)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		iRetByte = 128;
		break;

	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		iRetByte = 96;
		break;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		iRetByte = 64;
		break;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
	case DXGI_FORMAT_B8G8R8X8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
	case DXGI_FORMAT_B5G6R5_UNORM:
	case DXGI_FORMAT_B5G5R5A1_UNORM:
		iRetByte = 16;
		break;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		iRetByte = 8;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC5_TYPELESS:
	case DXGI_FORMAT_BC5_UNORM:
	case DXGI_FORMAT_BC5_SNORM:
		iRetByte = 128;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R1_UNORM:
	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC4_TYPELESS:
	case DXGI_FORMAT_BC4_UNORM:
	case DXGI_FORMAT_BC4_SNORM:
		iRetByte = 64;
		break;
		// Compressed format; http://msdn2.microsoft.com/en-us/library/bb694531(VS.85).aspx
	case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		iRetByte = 32;
		break;
		// These are compressed, but bit-size information is unclear.
	case DXGI_FORMAT_R8G8_B8G8_UNORM:
	case DXGI_FORMAT_G8R8_G8B8_UNORM:
		iRetByte = 32;
		break;
	case DXGI_FORMAT_UNKNOWN:
	default:
		return -1;
	}

	return iRetByte / 8;
}

void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName)
{
	D3D11_INPUT_ELEMENT_DESC LayoutDesc = {};
	LayoutDesc.AlignedByteOffset = m_iLayoutOffset;
	LayoutDesc.Format = _eFormat;
	LayoutDesc.InputSlot = 0;
	LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc.SemanticName = _strSemanticName;
	LayoutDesc.SemanticIndex = 0;
	m_vecLayoutInfo.push_back(LayoutDesc);

	m_iLayoutOffset += GetSizeofFormat(_eFormat);
}