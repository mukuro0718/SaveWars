#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Audience.h"
#include "LoadingAsset.h"
#include "InputManager.h"
#include "PlayerManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Audience::Audience()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& json  = Singleton<JsonManager>::GetInstance();

	VERTEX3D base;
	base.pos = Gori::ORIGIN;
	base.norm = VGet(0.0, 0.0, 1.0);
	base.dif = GetColorU8(255, 255, 255, 255);
	base.spc = GetColorU8(0, 0, 0, 0);
	base.u = 0.0f;
	base.v = 0.0f;
	base.su = 0.0f;
	base.sv = 0.0f;
	for (int i = 0; i < this->VERTEX_NUM; i++)
	{
		this->vertex[i] = base;
		this->vertex[i].pos = Convert(json.GetJson(JsonManager::FileType::UI)["AUDIENCE_VERTEX_POSITION"][i]);
	}

	for (int i = 0; i < this->VERTEX_NUM; i++)
	{
		this->index[i] = json.GetJson(JsonManager::FileType::UI)["AUDIENCE_VERTEX_INDEX"][i];
	}
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::AUDIENCE);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Audience::~Audience()
{

}

/// <summary>
/// ������
/// </summary>
void Audience::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*������*/
}

/// <summary>
/// �X�V
/// </summary>
void Audience::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();


}

/// <summary>
/// �`��
/// </summary>
void Audience::Draw()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	//DrawPolygonIndexed3D(this->vertex, this->VERTEX_NUM, this->index, this->POLYGON_NUM, DX_NONE_GRAPH/*this->imageHandle*/, TRUE);
}

/// <summary>
/// �F�擾
/// </summary>
int Audience::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}

/// <summary>
/// VECTOR�ɕϊ�
/// </summary>
VECTOR Audience::Convert(const std::vector<int> _in)
{
	VECTOR out;
	out.x = _in[0];
	out.y = _in[1];
	out.z = _in[2];

	return out;
}
