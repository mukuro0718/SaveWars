//=================================================
// @brief �{�X�}�l�[�W���[�N���X
//=================================================
#pragma once
#include "Singleton.h"

class Character;
class CharacterData;
class Rigidbody;
class EnemyManager : public Singleton<EnemyManager>
{
public:
	friend class Singleton<EnemyManager>;

	void Initialize(); //������
	void Finalize();
	void Update(); //�X�V
	const void Draw		 ()const;

	/*getter*/
	const int	   GetHP()const;
	const CharacterData& GetCharacterData()const;
	const Rigidbody& GetRigidbody()const;
	const bool IsAttack()const;
	const int GetModelHandle()const;
	const bool GetIsAlive()const;
private:
	/*���������֐�*/
	EnemyManager();//�R���X�g���N�^
	~EnemyManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	std::vector<Character*> boss;//�{�X
	int enemyType;
};

