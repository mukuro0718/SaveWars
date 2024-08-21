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
	const void Draw		 ()const;//�`��

	/*getter*/
	const VECTOR   GetPosition			()const;//���W�̎擾
	const VECTOR   GetDirection			()const;//���W�̎擾
	const bool     IsMove				()const;//�ړ�������
	const bool	   IsAttack				()const;//�U��������
	const bool	   IsShowElementWheel	()const;//�����z�C�[����\��������
	const Collider GetCollider			();
	const int	   GetHP				()const;
	const int	   GetCurrentElement	()const;
	const float GetElementAngle()const;

private:
	/*���������֐�*/
	PlayerManager();//�R���X�g���N�^
	~PlayerManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	Player* player;//�v���C���[
};

