//=================================================
// @brief �{�X�}�l�[�W���[�N���X
//=================================================
#pragma once
#include "Singleton.h"

class Boss;
class Collider;
class EnemyManager : public Singleton<EnemyManager>
{
public:
	friend class Singleton<EnemyManager>;

	void Initialize();
	void Update();
	const void Draw()const;

	/*getter*/
	const VECTOR GetPosition()const;//���W�̎擾
	const Collider GetCollider();
private:
	/*���������֐�*/
	EnemyManager();//�R���X�g���N�^
	~EnemyManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	Boss* boss;//�{�X
};

