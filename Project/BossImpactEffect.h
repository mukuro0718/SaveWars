//=====================================
// @brief�@�{�X�U���q�b�g���G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BossImpactEffect : public Effect
{
public:
	BossImpactEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BossImpactEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

