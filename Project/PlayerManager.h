//=================================================
// @brief �v���C���[�}�l�[�W���[�N���X
//=================================================
#pragma once
#include "Singleton.h"

class Player;
class Collider;
class PlayerManager : public Singleton<PlayerManager>
{
public:
	friend class Singleton<PlayerManager>;

		  void Initialize();	 //������
		  void Update	 ();	 //�X�V
		  void Action	 ();	 //�A�N�V����
		  void		FixMoveVector(const VECTOR _fixVector);				  //�ړ��x�N�g���̏C��
		  const void Draw		 ()const;//�`��

	/*getter*/
		  void CalcDamage(const int _damage);
	const int GetDamage()const;
	const VECTOR   GetPosition			()const;//���W�̎擾
	const VECTOR   GetDirection			()const;//���W�̎擾
	const bool     IsMove				()const;//�ړ�������
	const bool	   IsAttack				()const;//�U��������
	const Collider GetCharacterCollider();
	const Collider GetAttackCollider();
	const int	   GetHP				()const;
	const int GetAttackNumber()const;
	const int GetHitNumber()const;
	void SetHitNumber(const int _attackNumber);

private:
	/*���������֐�*/
	PlayerManager();//�R���X�g���N�^
	~PlayerManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	Player* player;//�v���C���[
};

