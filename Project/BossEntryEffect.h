//=====================================
// @brief�@�{�X�G���g���[�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BossEntryEffect : public Effect
{
public:
	BossEntryEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BossEntryEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

