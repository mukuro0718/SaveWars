#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "GameClearScene.h"
#include "LoadingAsset.h"
#include "VECTORtoUseful.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameClearScene::GameClearScene()
	: FIRST_LOGO_ANGLE								(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_FIRST_LOGO_ANGLE"])
	, TARGET_LOGO_ANGLE								(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TARGET_LOGO_ANGLE"])
	, FIRST_LOGO_RATE								(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_FIRST_LOGO_RATE"])
	, TARGET_LOGO_RATE_FOR_FADE_IN					(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TARGET_LOGO_RATE_WITH_FADE_IN"])
	, TARGET_LOGO_RATE_FOR_FADE_OUT					(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TARGET_LOGO_RATE_WITH_FADE_OUT"])
	, TARGET_EFFECT_LOGO_RATE						(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_TARGET_EFFECT_LOGO_RATE"])
	, LERP_VALUE_FOR_ANGLE							(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_LERP_VALUE_FOR_ANGLE"])
	, LERP_VALUE_FOR_REDUCTION						(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_LERP_VALUE_FOR_REDUCTION"])
	, LERP_VALUE_FOR_FADE_OUT_MAGNIFICATION			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_LERP_VALUE_FOR_FADE_OUT_MAGNIFICATION"])
	, LERP_VALUE_FOR_EFFECT_FADE_OUT_MAGNIFICATION	(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_LERP_VALUE_FOR_EFFECT_FADE_OUT_MAGNIFIGATION"])
	, WAIT_TIME_BEFORE_START_FADE_IN				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WAIT_TIME_BEFORE_START_FADE_IN"])
	, WAIT_TIME_BEFORE_START_FADE_OUT				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_WAIT_TIME_BEFORE_START_FADE_OUT"])
	, INCREASED_ALPHA_FOR_LOGO_FADE_IN				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_INCREASED_ALPHA_FOR_LOGO_FADE_IN"])
	, REDUCTION_ALPHA_FOR_LOGO_FADE_OUT				(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_REDUCTION_ALPHA_FOR_LOGO_FADE_OUT"])
	, REDUCTION_ALPHA_FOR_EFFECT_FADE_OUT			(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::UI)["GAME_CLEAR_REDUCTION_ALPHA_FOR_EFFECT_FADE_OUT"])
{
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->questClearLogo = asset.GetImage(LoadingAsset::ImageType::QUEST_CLEAR);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameClearScene::~GameClearScene()
{
}

/// <summary>
/// ������
/// </summary>
void GameClearScene::Initialize()
{
	this->logoAngle			= this->FIRST_LOGO_ANGLE;
	this->logoRate			= this->FIRST_LOGO_RATE;
	this->logoAlpha			= 0;
	this->effectLogoRate	= 0.0f;
	this->effectLogoAlpha	= 0;
	this->waitTimer			= this->WAIT_TIME_BEFORE_START_FADE_IN;
	this->isProductFinished = false;
}

/// <summary>
/// �X�V
/// </summary>
void GameClearScene::Update()
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
		//��]����
		this->logoAngle = Gori::LerpFloat(this->logoAngle, this->TARGET_LOGO_ANGLE, this->LERP_VALUE_FOR_ANGLE);
		//�k������
		this->logoRate = Gori::LerpFloat(this->logoRate, this->TARGET_LOGO_RATE_FOR_FADE_IN, this->LERP_VALUE_FOR_REDUCTION);
		//�t�F�[�h�C��
		this->logoAlpha++;
		//�t�F�[�h�C�����I����Ă�����i�K��؂�Ԃ�
		if (this->logoAlpha == this->MAX_ALPHA)
		{
			this->currentProductStage = ProductStage::EFFECT_FADE_OUT;
			this->effectLogoAlpha	  = this->logoAlpha;
			this->effectLogoRate	  = this->logoRate;
		}
		break;
	case ProductStage::EFFECT_FADE_OUT:
		//�g�又��
		this->effectLogoRate = Gori::LerpFloat(this->effectLogoRate, this->TARGET_EFFECT_LOGO_RATE, this->LERP_VALUE_FOR_EFFECT_FADE_OUT_MAGNIFICATION);
		//�t�F�[�h�A�E�g
		this->effectLogoAlpha++;
		//�t�F�[�h�A�E�g���I����Ă�����i�K��؂�Ԃ�
		if (this->effectLogoAlpha == 0)
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
		//�g�又��
		this->logoRate = Gori::LerpFloat(this->effectLogoRate, this->TARGET_LOGO_RATE_FOR_FADE_OUT, this->LERP_VALUE_FOR_FADE_OUT_MAGNIFICATION);
		//�t�F�[�h�A�E�g
		this->logoAlpha++;
		//�t�F�[�h�A�E�g���I����Ă�����i�K��؂�Ԃ�
		if (this->logoAlpha == 0)
		{
			this->currentProductStage = ProductStage::WAIT_FOR_FADE_IN;
			this->isProductFinished	  = true;
		}
		break;
	}
}

/// <summary>
/// �`��
/// </summary>
const void GameClearScene::Draw()const
{
	/*�I���t���O�������Ă����瑁�����^�[��*/
	if (this->isProductFinished)return;

	/*���S�̕`��*/
	double rate = static_cast<double>(this->logoRate);
	double angle = static_cast<double>(this->logoAngle);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->logoAlpha);
	DrawRotaGraph(this->DRAW_POSITION_X, DRAW_POSITION_Y, rate, angle, this->questClearLogo, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->MAX_ALPHA);

	/*�G�t�F�N�g�p���S�̕`��*/
	if (this->currentProductStage == ProductStage::EFFECT_FADE_OUT)
	{
		double rate = static_cast<double>(this->effectLogoRate);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, this->effectLogoAlpha);
		DrawRotaGraph(this->DRAW_POSITION_X, DRAW_POSITION_Y, rate, angle, this->questClearLogo, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, this->MAX_ALPHA);
	}
}