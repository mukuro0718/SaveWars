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
		  void Update	 ();
		  void Action	 ();	 //�A�N�V����
	const void Draw		 ()const;

	/*getter*/
	const int	   GetHP()const;
	const VECTOR GetPosition()const;//���W�̎擾
	const Collider GetCharacterCollider();
	const Collider GetAttackCollider();
	void CalcDamage(const int _damage);
	const int GetDamage()const;
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

