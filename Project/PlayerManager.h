//=================================================
// @brief �v���C���[�}�l�[�W���[�N���X
//=================================================
#pragma once
#include "Singleton.h"

class Player;
class PlayerManager : public Singleton<PlayerManager>
{
public:
	friend class Singleton<PlayerManager>;

		  void Initialize(GoriLib::Physics* _physics); //������
		  void Finalize  (GoriLib::Physics* _physics);
		  void Update	 (GoriLib::Physics* _physics); //�X�V
		  void OnCollide (const GoriLib::Collidable& _colider); //�ړ��x�N�g���̏C��
	const void Draw		 ()const;					   //�`��

	/*getter*/
		  void CalcDamage(const int _damage);
	const int GetDamage()const;
	const VECTOR   GetPosition			()const;//���W�̎擾
	const VECTOR   GetDirection			()const;//���W�̎擾
	const bool     IsMove				()const;//�ړ�������
	const bool	   IsAttack				()const;//�U��������
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

