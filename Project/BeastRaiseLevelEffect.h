//=====================================
// @brief�@�b�{�X���x���㏸�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BeastRaiseLevelEffect : public Effect
{
public:
	 BeastRaiseLevelEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BeastRaiseLevelEffect() {}//�f�X�g���N�^

	void Update() override;//�X�V
private:
	VECTOR	scale;				//�g�嗦
	int		playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
	short	useFrameIndex;		//�g�p����t���[���ԍ�
};

