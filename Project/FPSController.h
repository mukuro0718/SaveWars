//===========================================================================
//@brief FPS�R���g���[���[�N���X
// FPS�̐ݒ�A�v�Z�ƕ`����s��
//===========================================================================
#pragma once
#include "Singleton.h"

class FPSController : public Singleton<FPSController>
{
public:
	friend class Singleton<FPSController>;

	void Initialize();		//������
	void CalcStartTime();

	void Update();			//FPS�̍X�V
	void UpdateTargetFPS(); //�ڕWFPS�̍X�V
	const void Draw()const;

private:
	static constexpr int NORMAL_TIME_SCALE = 1000;

	FPSController();
	~FPSController();

	/*�����o�ϐ�*/
	int		targetFPS;	//�ڕWFPS
	double	fps;		//fps
	int		startTime;	//�v���J�n����
	int		timeScale;	//���Ԃ̃X�P�[��(1000 = 1�b)
	int		frameCount;	//�t���[���J�E���g
	int		lastTime;	//�Ō�̎���
};