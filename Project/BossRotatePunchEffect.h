//=====================================
// @brief�@�{�X�p���`�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BossRotatePunchEffect : public Effect
{
public:
	BossRotatePunchEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BossRotatePunchEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

