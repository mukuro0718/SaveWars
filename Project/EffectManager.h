#pragma once
#include "Singleton.h"

class EffectManager : public Singleton<EffectManager>
{
public:
	friend class Singleton<EffectManager>;

	/*�G�t�F�N�g�̕`��p�֐�*/
	void AddEffect();
	void RemoveEffect();
	void OnIsPlayEffect(const int _in){}

	void Initialize();
	void Finalize();

private:
	/*���������֐�*/
	EffectManager();//�R���X�g���N�^
	~EffectManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
};

