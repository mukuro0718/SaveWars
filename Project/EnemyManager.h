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

	void Initialize(GoriLib::Physics* _physics); //������
	void Finalize(GoriLib::Physics* _physics);
	void Update(GoriLib::Physics* _physics); //�X�V
	void OnCollide(const GoriLib::Collidable& _colider); //�ړ��x�N�g���̏C��
	const void Draw		 ()const;

	/*getter*/
	const int	   GetHP()const;
	const VECTOR GetPosition()const;//���W�̎擾
	const VECTOR GetHeadPosition()const;//���W�̎擾
	const int GetAttackNumber()const;
	const int GetHitNumber()const;
	void SetHitNumber(const int _attackNumber);
	const bool IsAttack()const;
	void OnIsHitAttack();
private:
	/*���������֐�*/
	EnemyManager();//�R���X�g���N�^
	~EnemyManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	Boss* boss;//�{�X
};

