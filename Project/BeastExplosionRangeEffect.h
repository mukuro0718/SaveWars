//=====================================
// @brief�@�b�{�X�����͈̓G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BeastExplosionRangeEffect : public Effect
{
public:
	 BeastExplosionRangeEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BeastExplosionRangeEffect() {}//�f�X�g���N�^

	void Update() override;//�X�V
private:
	VECTOR	scale;				//�g�嗦
	int		playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
	short	useIndexNum;		//�g�p����t���[���̔ԍ�
};

