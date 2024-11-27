#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "SelectUI.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"
#include "EnemyChanger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SelectUI::SelectUI()
{
	/*�摜�n���h���̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->background = asset.GetImage(LoadingAsset::ImageType::SELECT_BACK);
	this->logo = asset.GetImage(LoadingAsset::ImageType::SELECT_LOGO);
	this->provDecide = asset.GetImage(LoadingAsset::ImageType::SELECT_PROV_DECIDE);
	this->icon.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_ICON_1));
	this->icon.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_ICON_2));
	this->icon.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_ICON_3));
	this->icon.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_ICON_4));
	this->image.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_1));
	this->image.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_2));
	this->image.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_3));
	this->image.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_IMAGE_4));
	this->summary.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_SUMMARY_1));
	this->summary.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_SUMMARY_2));
	this->summary.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_SUMMARY_3));
	this->summary.emplace_back(asset.GetImage(LoadingAsset::ImageType::QUEST_SUMMARY_4));
	this->iconFrame = asset.GetImage(LoadingAsset::ImageType::SELECT_ICON_FRAME);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SelectUI::~SelectUI()
{
	this->icon.clear();
	this->image.clear();
	this->summary.clear();
}

/// <summary>
/// ������
/// </summary>
void SelectUI::Initialize()
{
	/*�G�l�~�[�`�F���W���[�̏�����*/
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	enemyChanger.Initialize();
}


/// <summary>
/// �X�V
/// </summary>
void SelectUI::Update()
{
	/*�G�l�~�[�`�F���W���[�̍X�V*/
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	enemyChanger.Update();
	this->nowSelectEnemy = enemyChanger.GetEnemyType();

}

/// <summary>
/// �`��
/// </summary>
const void SelectUI::Draw()const
{
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�w�i*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_BACKGROUND_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->background, TRUE);
	}

	/*�C���[�W�摜*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_IMAGE_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->image[this->nowSelectEnemy], TRUE);
	}

	/*���S*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_LOGO_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->logo, TRUE);
	}

	/*�A�C�R��*/
	{
		vector<vector<int>> position = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_POSITION"];
		for (int i = 0; i < position.size(); i++)
		{
			DrawExtendGraph(position[i][0], position[i][1], position[i][2], position[i][3], this->icon[i], TRUE);
		}
	}

	/*�t���[��*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_ICON_POSITION"][this->nowSelectEnemy];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->iconFrame, TRUE);
	}

	/*�T�}���[*/
	{
		vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_SUMMARY_POSITION"];
		DrawExtendGraph(position[0], position[1], position[2], position[3], this->summary[this->nowSelectEnemy], TRUE);
	}

	/*�ŏI����*/
	{
		auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
		if (enemyChanger.GetIsProvDecide())
		{
			vector<int> position = json.GetJson(JsonManager::FileType::UI)["SELECT_PROV_POSITION"];
			DrawExtendGraph(position[0], position[1], position[2], position[3], this->provDecide, TRUE);
		}
	}
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
const bool SelectUI::IsEnd()const
{
	auto& enemyChanger = Singleton<EnemyChanger>::GetInstance();
	return enemyChanger.GetIsFinalDecide();
}