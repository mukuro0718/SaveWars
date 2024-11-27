//=============================================
// @brief �G�l�~�[�`�F���W���[�N���X
// �o�ꂷ��G�l�~�[�̕ύX���s��
//=============================================
#pragma once
#include "Singleton.h"

class EnemyChanger : public Singleton<EnemyChanger>
{
public:
	friend class Singleton<EnemyChanger>;
	
	/*�G�l�~�[�̎��*/
	enum class EnemyType
	{
		GOLEM	 = 0,//�S�[����
		BEAST	 = 1,//�b
		DRAGON	 = 2,//�h���S��
		TUTORIAL = 3,//�`���[�g���A��
	};

	void Initialize();//������
	void Update();//

	const int GetEnemyType()const { return this->enemyType; }
	const bool GetIsProvDecide()const { return this->isProvDecide; }
	const bool GetIsFinalDecide()const { return this->isFinalDecide; }
private:
	 EnemyChanger();
	~EnemyChanger();

	/*�ÓI�萔*/
	static constexpr int MAX_INPUT_INTERBVAL = 10;//�ő���̓C���^�[�o��

	/*�����o�ϐ�*/
	int	enemyType;				//�^�C�v
	int selectType;
	int	inputInterval;		//���̓C���^�[�o��
	bool isProvDecide;		//���̌���
	bool isFinalDecide;		//�ŏI����
};

