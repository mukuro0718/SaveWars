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

	void Update() override;//�X�V
private:
	float	yOffset;			//Y�I�t�Z�b�g
	VECTOR	scale;				//�g�嗦
	int		playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
};

