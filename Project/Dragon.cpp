#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "Animation.h"
#include "Character.h"
#include "Enemy.h"
#include "Player.h"
#include "DragonBehaviorTreeHeader.h"
#include "Dragon.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "EffectManager.h"
#include "Debug.h"
#include "HitStopManager.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Dragon::Dragon()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::DRAGON));

	/*�A�j���[�V�����̐ݒ�*/
	vector<int>	animationHandle		  = json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_HANDLE"];
	vector<int>		animationIndex	  = json.GetJson(JsonManager::FileType::DRAGON)["ANIMATION_INDEX"];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(animationHandle[i], animationIndex[i]);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);

	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, new CharacterData());
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Dragon::~Dragon()
{
	Finalize();
}

void Dragon::Initialize()
{
	/*�ϐ��̏�����*/
	auto& json = Singleton<JsonManager>::GetInstance();
	this->isAlive		= true;
	this->isGround		= true;
	this->isDraw		= true;
	this->speed			= 0.0f;
	this->entryInterval	= 0;
	
	/*�R���C�_�[�̏�����*/
	const VECTOR POSITION = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_POSITION"]);//���W
	const VECTOR ROTATION = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_ROTATION"]);//��]��
	const VECTOR SCALE = Gori::Convert(json.GetJson(JsonManager::FileType::DRAGON)["INIT_SCALE"]);	 //�g�嗦
	float height						= json.GetJson(JsonManager::FileType::DRAGON)["HIT_HEIGHT"];
	this->collider->topPositon			= VGet(0.0f, height, 0.0f);
	this->collider->radius				= json.GetJson(JsonManager::FileType::DRAGON)["HIT_RADIUS"];
	this->collider->isUseCollWithGround = true;
	this->collider->data->hp			= json.GetJson(JsonManager::FileType::DRAGON)["HP"];
	this->collider->data->isHit			= false;
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);

	/*���f���̏�����*/
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	/*�A�j���[�V�����̏�����*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// �㏈��
/// </summary>
void Dragon::Finalize()
{
}

/// <summary>
/// �X�V
/// </summary>
void Dragon::Update()
{
	/*TODO:�����Ńr�w�C�r�A�c���[�̍X�V���s���B
	�܂�������DragonState��n���B*/

}

/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
void Dragon::PlayAnimation(const float _playAnimation, const float _playTime)
{
	//�A�j���[�V�����̍Đ�
	if (this->isAlive)
	{
		this->animation->Play(&this->modelHandle, _playAnimation, _playTime);
	}
}

/// <summary>
/// �`��
/// </summary>
const void Dragon::DrawCharacterInfo()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& debug = Singleton<Debug>::GetInstance();
	
	if (this->isDraw)
	{
		/*�����̕`��*/
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
}

const bool Dragon::GetIsAttack()const
{
	return false;
}
