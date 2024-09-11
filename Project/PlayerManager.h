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
	const CharacterData& GetCharacterData()const;
	const Rigidbody&	 GetRigidbody	 ()const;
	const bool			 IsMove			 ()const;//�ړ�������
	const bool			 GetIsAttack	 ()const;//�U��������
	const int			 GetHP			 ()const;
	const float			 GetStamina		 ()const;
	const int			 GetModelHandle	 ()const;
private:
	/*���������֐�*/
	PlayerManager();//�R���X�g���N�^
	~PlayerManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	Character* player;//�v���C���[
};

