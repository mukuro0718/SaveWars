//=====================================
// @brief�@�{�X�؂���G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BossSlashEffect : public Effect
{
public:
	BossSlashEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BossSlashEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

