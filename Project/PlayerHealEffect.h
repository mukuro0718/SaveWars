//=====================================
// @brief�@�񕜃G�t�F�N�g
//=====================================
#pragma once

class Effect;
class PlayerHealEffect : public Effect
{
public:
	PlayerHealEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~PlayerHealEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

