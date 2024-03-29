#pragma once

struct tRGBA
{
	int R;
	int G;
	int B;
	int A;
};

struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

struct tEvent
{
	EVENT_TYPE eType;
	DWORD_PTR  wParam;
	DWORD_PTR  lParam;
	DWORD_PTR  oParam;
};

// ==============
// TileMap 구조체
// ==============
struct tTile
{
	Vec3		vPos;
	UINT        iIndex;
	UINT		iInfo;
};

struct tAnim2DFrm
{
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;
	float fDuration;
};

struct tAnim2DInfo
{
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;

	int iAnim2DUse;
	/*
	* .fx 파일에서 16바이트 단위로 데이터 전달 필요
	*/
	int iPaading[3];
};
struct tDebugShapeInfo
{
	DEBUG_SHAPE eShape;
	Vec4		vColor;
	Vec3		vPosition;
	Vec3		vScale;
	Vec3		vRot;
	float		fRadius;
	float		fDuration;
	float		fCurTime;
};

// ================
// Shader Parameter
// ================
/*
* Shader 생성 시, 사용 Scalar, Texture Param Setting
* Material UI -> Material -> GetShader -> GetScalarParam, TextureParam
*/
struct tScalarParam
{
	SCALAR_PARAM eParam;
	string		 strName;
};

struct tTextureParam
{
	TEX_PARAM	 eParam;
	string		 strName;
};

// ================
// 상수버퍼용 구조체
// ================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

struct tMtrlConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int HasTex[(UINT)TEX_PARAM::TEX_END];
};

struct tObjectRender
{
	tTransform transform;
	tMtrlConst mtrl;
	tAnim2DInfo animation;
};

struct tLightInfo
{
	Vec4		vDiff;
	Vec4		vSpec;
	Vec4		vEmv;

	Vec4		vWorldPos;
	Vec4		vWorldDir;
	LIGHT_TYPE  iLightType;
	float		fRadius;
	float		fAngle;
	int			pad;
};

struct tGlobalData
{
	Vec2	vRenderResolution;
	Vec2	vNoiseResolution;
	int		iLight2DCount;
	int		iLight3DCount;

	float	fAccTime;
	float	fDT;
};

struct tParticle
{
	Vec4 vRelativePos;
	Vec4 vDir;

	float fMaxTime;
	float fCurTime;
	float fSpeed;
	UINT iActive;
};

struct tParticleShare
{
	UINT iAliveCount;
	UINT iPadding[3];
};

struct tWoodWall
{
	bool bChecked = false;
	bool arr[8] = { false, };
};

struct tDxatlas
{
	UINT  iFrame{};
	float x = 0.f;
	float y = 0.f;
	float w = 0.f;
	float h = 0.f;

	float pX = 0.f;
	float pY = 0.f;
	float oX = 0.f;
	float oY = 0.f;

	float oW = 0.f;
	float oH = 0.f;
};

struct tBlock
{
	int x;
	int z;
};

struct tTextInfo
{
	wchar_t sz[50];
	Vec2 vPos;
	float fSize;
	Vec4 vColor;
};

extern tGlobalData g_global;
extern  tTransform g_transform;
extern  std::vector<tObjectRender>	g_vecInfoObject;