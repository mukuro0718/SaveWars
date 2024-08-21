#include <DxLib.h>
#include "UseSTL.h"
#include "LoadingAsset.h"
#include "Action.h"
#include "PlayerManager.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Action::Action()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->elementImageHandle[0] = asset.GetImage(LoadingAsset::ImageType::FRAME_ICON);
	this->elementImageHandle[1] = asset.GetImage(LoadingAsset::ImageType::BOLT_ICON);
	this->elementImageHandle[2] = asset.GetImage(LoadingAsset::ImageType::ICE_ICON);
	this->elementImageHandle[3] = asset.GetImage(LoadingAsset::ImageType::EARTH_ICON);
	this->elementImageHandle[4] = asset.GetImage(LoadingAsset::ImageType::STORM_ICON);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Action::~Action()
{

}

/// <summary>
/// �`��
/// </summary>
const void Action::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	const int NUM_VERTEX = this->ELEMENT_NUM;
	const Vector2 FIRST_POS = { 960,440 };
	const Vector2 CENTER_POS = { 960,440 };
	const float RADIUS = 150.0f;
	const int HALF_SIZE = 50;
	const float ANGLE_OFFSET = 45.0f;
	
	/*�����z�C�[���\���t���O�������Ă�����*/
	if (player.IsShowElementWheel())
	{
		for (int i = 0; i < NUM_VERTEX; i++)
		{
			float angle = (2.0f * DX_PI_F * i / NUM_VERTEX) + ANGLE_OFFSET * (180.0f / DX_PI_F);
			int x = static_cast<int>(RADIUS * cosf(angle)) + FIRST_POS.x;
			int y = static_cast<int>(RADIUS * sinf(angle)) + FIRST_POS.y;
			DrawExtendGraph(x - HALF_SIZE, y - HALF_SIZE, x + HALF_SIZE, y + HALF_SIZE, this->elementImageHandle[i], TRUE);
		}
	}

	/*�o�[�̕\��*/
	Vector2 buf = { input.GetRStickState().XBuf ,input.GetRStickState().YBuf };
	DrawLine(CENTER_POS.x, CENTER_POS.y, CENTER_POS.x + buf.x, CENTER_POS.y + buf.y, GetColor(200, 200, 50), TRUE);

	/*show element*/
	int nowElement = player.GetCurrentElement();
	DrawBox(50, 300, 150, 400, GetColor(255,255,0), TRUE);
	DrawExtendGraph(50, 300, 150, 400, this->elementImageHandle[nowElement], TRUE);
}