//=====================================
// @brief�@�v���C���[�G���g���[�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class PlayerEntryEffect : public Effect
{
public:
	 PlayerEntryEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~PlayerEntryEffect(){}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

