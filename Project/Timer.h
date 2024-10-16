//===========================================================================
//@brief �^�C�}�[�N���X
// �Q�[���J�n���ɏ��������A�Q�[�����v��������
//===========================================================================
#pragma once
#include "Singleton.h"

class Timer : public Singleton<Timer>
{
public:
	friend class Singleton<Timer>;

	void Initialize() { this->time = 0; }		//������

	void Update() { this->time++; } //�ڕWFPS�̍X�V

	const void Draw() const;

	const int GetNowTime()const { return this->time; }
private:

	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*���������֐�*/
	Timer();
	~Timer(){}

	/*�����o�ϐ�*/
	int		time;
	int fontHandle;
};