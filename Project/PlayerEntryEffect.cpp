//#include <DxLib.h>
//#include "EffekseerForDXLib.h"
//#include "UseSTL.h"
//#include "UseJson.h"
//#include "Transform.h"
//#include "Rigidbody.h"
//#include "Effect.h"
//#include "PlayerEntryEffect.h"
//#include "PlayerManager.h"
//
//PlayerEntryEffect::PlayerEntryEffect(const int _effectResourceHandle)
//	: Effect(_effectResourceHandle)
//{
//}
//
///// <summary>
///// �G�t�F�N�g�̍Đ��ɕK�v�ȏ��̐ݒ�
///// </summary>
//void PlayerEntryEffect::SetInfoToPlayTheEffect()
//{
//	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
//	auto& json = Singleton<JsonManager>::GetInstance();
//	auto& player = Singleton<PlayerManager>::GetInstance();
//
//	this->transform->SetPosition(player.GetRigidbody().GetPosition());
//	this->transform->SetScale(Convert(json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_ENTRY_SCALE"]));
//	this->transform->SetRotation(player.GetRigidbody().GetRotation());
//	
//	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_ENTRY_START_FRAME"];
//	this->endFrame   = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_ENTRY_END_FRAME"];
//}
//
