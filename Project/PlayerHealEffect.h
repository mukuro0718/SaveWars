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

	void Update() override;//�X�V
private:
	VECTOR	scale;				//�g�嗦
	int		playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
};

