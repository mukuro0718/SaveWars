#include <DxLib.h>
#include "USEJson.h"
#include "UseSTL.h"
#include "Debug.h"
#include "InputManager.h"
#include "BitFlag.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Debug::Debug()
	: isReloadingJsonFile(false)
{
	this->jsonIndexMap.emplace(static_cast<int>(ItemType::CAMERA)	, static_cast<int>(JsonManager::FileType::CAMERA));
	this->jsonIndexMap.emplace(static_cast<int>(ItemType::PLAYER)	, static_cast<int>(JsonManager::FileType::PLAYER));
	this->jsonIndexMap.emplace(static_cast<int>(ItemType::FPS)		, static_cast<int>(JsonManager::FileType::FPS_CONTROLLER));
	this->jsonIndexMap.emplace(static_cast<int>(ItemType::ENEMY)	, static_cast<int>(JsonManager::FileType::ENEMY));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Debug::~Debug() 
{
	this->jsonIndexMap.clear();
}

/// <summary>
/// ������
/// </summary>
void Debug::Initialize()
{
}

/// <summary>
/// �X�V
/// </summary>
void Debug::Update()
{
	/*Json�t�@�C�����ēǂݍ���*/
	this->ReloadingJsonFile();
}

/// <summary>
/// Json�t�@�C�����ēǂݍ���
/// </summary>
void Debug::ReloadingJsonFile()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	/*����Json�t�@�C�����ēǂݍ��݂��Ă��Ȃ�������i���������ɂ��A���ǂݍ��݂�h���j*/
	if (!this->isReloadingJsonFile)
	{
		if (CheckHitKey(KEY_INPUT_R))
		{
			//�ǂݍ��݃t���O�𗧂Ă�
			this->isReloadingJsonFile = true;
			//�f�o�b�NJson�t�@�C����ǂݍ���
			json.ReloadingJsonFile(JsonManager::FileType::DEBUG);
			//�f�o�b�N���g�p���Ȃ��ɂȂ��Ă����瑁�����^�[��
			if (!json.GetJson(JsonManager::FileType::DEBUG)["IS_USE_DEBUG_MODE"])return;
			//�t���O�������Ă�����݂̂̂��ēǂݍ���
			vector<bool> isShowDebugInfo = json.GetJson(JsonManager::FileType::DEBUG)["SHOW_DEBUG_INFO"];
			for (int i = 0; i < isShowDebugInfo.size(); i++)
			{
				//�t���O�������Ă��Ȃ���Α������^�[��
				if (!isShowDebugInfo[i]) return;
				//�w�肳�ꂽJson�t�@�C�����ēǂݍ���
				static_cast<JsonManager::FileType>(i);
				json.ReloadingJsonFile(static_cast<JsonManager::FileType>(this->jsonIndexMap[i]));
			}
		}
	}
	else
	{
		if (!CheckHitKey(KEY_INPUT_R))
		{
			this->isReloadingJsonFile = false;
		}
	}

}

/// <summary>
/// �f�o�b�N���[�h���g�p���邩
/// </summary>
const bool Debug::IsUseDebugMode() const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	return json.GetJson(JsonManager::FileType::DEBUG)["IS_USE_DEBUG_MODE"];
}

/// <summary>
/// �f�o�b�N���[�h���g�p���邩
/// </summary>
const bool Debug::IsShowDebugInfo(const ItemType _type) const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	const int TYPE = static_cast<int>(_type);
	return json.GetJson(JsonManager::FileType::DEBUG)["SHOW_DEBUG_INFO"][TYPE];
}