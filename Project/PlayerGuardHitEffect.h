//=====================================
// @brief�@�K�[�h���q�b�g�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class PlayerGuardHitEffect : public Effect
{
public:
	PlayerGuardHitEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~PlayerGuardHitEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

