//=====================================
// @brief�@�b�{�X�唚���G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BeastSuperNovaEffect : public Effect
{
public:
	 BeastSuperNovaEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BeastSuperNovaEffect() {}//�f�X�g���N�^

	void Update() override;//�X�V
private:
	VECTOR	scale;				//�g�嗦
	int		playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
	short	useIndexNum;		//�g�p����t���[���̔ԍ�
};

