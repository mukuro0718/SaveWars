//=====================================
// @brief�@�{�X���K���G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BossRoarEffect : public Effect
{
public:
	 BossRoarEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BossRoarEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

