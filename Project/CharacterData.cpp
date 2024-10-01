#include <DxLib.h>
#include "ReactionType.h"
#include "CharacterData.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CharacterData::CharacterData()
	: hp(0)
	, isHit(false)
	, hitPosition(VGet(0.0f, 0.0f, 0.0f))
	, isCutDamage	(false)
	, playerReaction(0)
{

}