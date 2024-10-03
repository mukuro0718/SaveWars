//===========================================================================
//@brief FPS�R���g���[���[�N���X
// FPS�̐ݒ�A�v�Z�ƕ`����s��
// https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q10175479905�����p
//===========================================================================
#pragma once
#include "Singleton.h"

class FPSController : public Singleton<FPSController>
{
public:
	friend class Singleton<FPSController>;

	void Initialize();		//������
	void CalcStartTime();

	void Average();			//FPS�̕��ς��Z�o
	void Wait();			//�ڕWFPS�ɂȂ�悤�ҋ@
	void UpdateTargetFPS(); //�ڕWFPS�̍X�V
	const void Draw()const;

private:
	static constexpr int NORMAL_TIME_SCALE = 1000;

	FPSController();
	~FPSController();

	/*�����o�ϐ�*/
	int type;
	int		targetFPS;
	double	fps;		//fps
	int		startTime;//�v���J�n����
	int		count;	//������
	bool	isDebug;
	int timeScale;
};