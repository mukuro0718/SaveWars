//===========================================================================
//@brief FPS�R���g���[���[�N���X
// FPS�̐ݒ�A�v�Z�ƕ`����s��
// https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q10175479905�����p
//===========================================================================
#pragma once

class FPSController
{
public:
	FPSController();
	~FPSController();
	void Initialize();		//������
	void Average();			//FPS�̕��ς��Z�o
	void Wait();			//�ڕWFPS�ɂȂ�悤�ҋ@
	void UpdateTargetFPS(); //�ڕWFPS�̍X�V
private:
	/*�����o�ϐ�*/
	double	fps;		//fps
	int		startTime;//�v���J�n����
	int		count;	//������
	int		targetFPS;
};