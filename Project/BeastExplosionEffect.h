//=====================================
// @brief�@�b�{�X�����G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BeastExplosionEffect : public Effect
{
public:
	 BeastExplosionEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BeastExplosionEffect() {}//�f�X�g���N�^

	void Update() override;//�X�V
private:
	VECTOR	scale;				//�g�嗦
	int		playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
	short	useIndexNum;		//�g�p����t���[���̔ԍ�
};

