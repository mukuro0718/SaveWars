//=================================================
// @brief �v���C���[�}�l�[�W���[�N���X
//=================================================
#pragma once
#include "Singleton.h"

class Character;
class CharacterData;
class Rigidbody;
class PlayerManager : public Singleton<PlayerManager>
{
public:
	friend class Singleton<PlayerManager>;

		  void Initialize(); //������
		  void Finalize  ();
		  void Update	 (); //�X�V
	const void Draw		 ()const;					   //�`��

	/*getter*/
	const int			 GetNowState	 ()const;
	const CharacterData& GetCharacterData()const;//�L�����N�^�[�f�[�^�̎擾
	const Rigidbody&	 GetRigidbody	 ()const;//���W�b�h�{�f�B�̎擾
	const int			 GetHP			 ()const;//HP�̎擾
	const float			 GetStamina		 ()const;//�X�^�~�i�̎擾
	const int			 GetHealOrbNum	 ()const;//�񕜉񐔂̎擾
	const int			 GetModelHandle	 ()const;//���f���n���h���̎擾
	const bool			 GetIsAlive		 ()const;//�����t���O�̎擾
	const bool			 GetIsLockOn	 ()const;//���b�N�I���t���O�̎擾
	const bool			 GetIsDrawSword	 ()const;//�����t���O�̎擾
private:
	/*���������֐�*/
	PlayerManager();//�R���X�g���N�^
	~PlayerManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	Character* player;//�v���C���[
};

