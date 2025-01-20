#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "GameOverUI.h"
#include "LoadingAsset.h"
#include "VECTORtoUseful.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameOverUI::GameOverUI()
	: FIRST_LOGO_RATE						(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_FIRST_LOGO_RATE"])
	, TARGET_LOGO_RATE_FOR_FADE_IN			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_TARGET_LOGO_RATE_FOR_FADE_IN"	])
	, LERP_VALUE_FOR_REDUCTION				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_LERP_VALUE_FOR_REDUCTION"	])
	, WAIT_TIME_BEFORE_START_FADE_IN		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_WAIT_TIME_BEFORE_START_FADE_IN"])
	, WAIT_TIME_BEFORE_START_FADE_OUT		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_WAIT_TIME_BEFORE_START_FADE_OUT"])
	, INCREASED_ALPHA_FOR_LOGO_FADE_IN		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_INCREASED_ALPHA_FOR_LOGO_FADE_IN"])
	, REDUCTION_ALPHA_FOR_LOGO_FADE_OUT		(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_REDUCTION_ALPHA_FOR_LOGO_FADE_OUT"])
	, POSITION_X							(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_FIRST_POSITION_X"])
	, FIRST_POSITION_Y						(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_FIRST_POSITION_Y"])
	, POSITION_Y_FOR_FADE_OUT_MAGNIFICATION	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_OVER_POSITION_Y_FOR_FADE_OUT_MAGNIFICATION"])
{
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->questFailedLogo = asset.GetImage(LoadingAsset::ImageType::QUEST_FAILED);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameOverUI::~GameOverUI()
{
}

/// <summary>
/// ������
/// </summary>
void GameOverUI::Initialize()
{
	this->logoRate				= this->FIRST_LOGO_RATE;
	this->logoAlpha				= 0;
	this->waitTimer				= this->WAIT_TIME_BEFORE_START_FADE_IN;
	this->isProductFinished		= false;
	this->positionY				= this->FIRST_POSITION_Y;
	this->currentProductStage	= ProductStage::WAIT_FOR_FADE_IN;
}

/// <summary>
/// �X�V
/// </summary>
void GameOverUI::Update()
{
	/*�I���t���O�������Ă����瑁�����^�[��*/
	if (this->isProductFinished)return;

	switch (this->currentProductStage)
	{
	case ProductStage::WAIT_FOR_FADE_IN:
		//waitTimer�����炷
		this->waitTimer--;
		//waitTimer��0��������i�K��؂�Ԃ�
		if (this->waitTimer == 0)
		{
			this->currentProductStage = ProductStage::LOGO_FADE_IN;
		}
		break;
	case ProductStage::LOGO_FADE_IN:
		//�k������
		this->logoRate = Gori::LerpFloat(this->logoRate, this->TARGET_LOGO_RATE_FOR_FADE_IN, this->LERP_VALUE_FOR_REDUCTION);
		//�t�F�[�h�C��
		this->logoAlpha += this->INCREASED_ALPHA_FOR_LOGO_FADE_IN;
		//�t�F�[�h�C�����I����Ă�����i�K��؂�Ԃ�
		if (this->logoAlpha >= this->MAX_ALPHA)
		{
			this->currentProductStage = ProductStage::WAIT_FOR_FADE_OUT;
			this->waitTimer = this->WAIT_TIME_BEFORE_START_FADE_OUT;
		}
		break;
	case ProductStage::WAIT_FOR_FADE_OUT:
		//waitTimer�����炷
		this->waitTimer--;
		//waitTimer��0��������i�K��؂�Ԃ�
		if (this->waitTimer == 0)
		{
			this->currentProductStage = ProductStage::LOGO_FADE_OUT;
		}
		break;
	case ProductStage::LOGO_FADE_OUT:
		//�ړ�
		this->positionY += this->POSITION_Y_FOR_FADE_OUT_MAGNIFICATION;
		//�t�F�[�h�A�E�g
		this->logoAlpha -= this->REDUCTION_ALPHA_FOR_LOGO_FADE_OUT;
		//�t�F�[�h�A�E�g���I����Ă�����i�K��؂�Ԃ�
		if (this->logoAlpha <= 0)
		{
			this->currentProductStage = ProductStage::WAIT_FOR_FADE_IN;
			this->isProductFinished = true;
		}
		break;
	}
}

/// <summary>
/// �`��
/// </summary>
const void GameOverUI::Draw()const
{
	/*�I���t���O�������Ă����瑁�����^�[��*/
	if (this->isProductFinished)return;

	/*���S�̕`��*/
	double rate = static_cast<double>(this->logoRate);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->logoAlpha);
	DrawRotaGraph(this->POSITION_X, this->positionY, rate, this->LOGO_ANGLE, this->questFailedLogo, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->MAX_ALPHA);
}