//#include <DxLib.h>
//#include <Effekseer.h>
//#include <EffekseerRendererDX11.h>
//#include "UseSTL.h"
//#include "UseJson.h"
//#include "Transform.h"
//#include "Rigidbody.h"
//#include "Effect.h"
//#include "BossEntryEffect.h"
//#include "EnemyManager.h"
//
///// <summary>
///// �R���X�g���N�^
///// </summary>
//BossEntryEffect::BossEntryEffect(Effekseer::ManagerRef& _manager, const EFK_CHAR* _path)
//	: Effect(_manager, _path)
//{
//}
//
///// <summary>
///// ������
///// </summary>
//void BossEntryEffect::Initialize()
//{
//	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
//	auto& json = Singleton<JsonManager>::GetInstance();
//	auto& enemy = Singleton<EnemyManager>::GetInstance();
//
//	this->transform->SetScale(Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ENTRY_SCALE"]));
//	this->transform->SetRotation(enemy.GetRigidbody().GetRotation());
//}
//
///// <summary>
///// �X�V
///// </summary>
//void BossEntryEffect::Update(Effekseer::ManagerRef& _manager)
//{
//	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
//	auto& json = Singleton<JsonManager>::GetInstance();
//	auto& enemy = Singleton<EnemyManager>::GetInstance();
//
//	/*���W�̐ݒ�*/
//	this->transform->SetPosition(enemy.GetRigidbody().GetPosition());
//
//	//_manager->SetLocation(this->handle,)
//}
//
