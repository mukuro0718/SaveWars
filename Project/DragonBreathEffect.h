//=====================================
// @brief�@�b�{�X�u���X�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class DragonBreathEffect : public Effect
{
public:
	 DragonBreathEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~DragonBreathEffect() {}//�f�X�g���N�^

	void Update() override;//�X�V
private:
	VECTOR			scale;				//�g�嗦
	vector<int>		playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
	vector<VECTOR>	position;			//���W
	vector<VECTOR>	velocity;			//���W
	vector<bool>	isPlay;				//�Ή�����G�t�F�N�g���Đ����Ă��邩
	short			useFrameIndex1;		//�g�p����t���[���̔ԍ�
	short			useFrameIndex2;		//�g�p����t���[���̔ԍ�
	short			maxEffectNum;		//�G�t�F�N�g�̐�
	short			useEffectNum;		//�g�p���Ă���G�t�F�N�g�̐�
	float			moveSpeed;			//�G�t�F�N�g�̈ړ��X�s�[�h
	float			maxDistance;		//�����ʒu����̍ő勗��
};

