//#include <DxLib.h>
//#include "EffekseerForDXLib.h"
//#include "UseSTL.h"
//#include "UseJson.h"
//#include "Transform.h"
//#include "Rigidbody.h"
//#include "Effect.h"
//#include "BossSlashEffect.h"
//#include "EnemyManager.h"
//
//BossSlashEffect::BossSlashEffect(const int _effectResourceHandle)
//	: Effect(_effectResourceHandle)
//{
//}
//
///// <summary>
///// �G�t�F�N�g�̍Đ��ɕK�v�ȏ��̐ݒ�
///// </summary>
//void BossSlashEffect::SetInfoToPlayTheEffect()
//{
//	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
//	auto& json = Singleton<JsonManager>::GetInstance();
//	auto& enemy = Singleton<EnemyManager>::GetInstance();
//
//	VECTOR position = enemy.GetRigidbody().GetPosition();
//	position.y += json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_Y_OFFSET"];
//	VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_SCALE"]);
//	VECTOR rotation = enemy.GetRigidbody().GetRotation();
//	//rotation.y += json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_ROTATE_OFFSET"] * 180.0f / DX_PI_F;
//	
//	this->transform->SetPosition(position);
//	this->transform->SetScale(scale);
//	this->transform->SetRotationOfRadian(rotation);
//	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_START_FRAME"];
//	this->endFrame   = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_END_FRAME"];
//}
//
