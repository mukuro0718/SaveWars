//=====================================
// @brief�@�v���C���[�̍U���q�b�g�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class PlayerImpactEffect : public Effect
{
public:
	PlayerImpactEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~PlayerImpactEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

