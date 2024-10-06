//==================================================
// @brief �f�o�b�N�N���X
// �f�o�b�O�@�\��ǉ�����
//==================================================
#pragma once
#include "Singleton.h"

class BitFlag;
class Debug : public Singleton<Debug>
{
public:
	friend class Singleton<Debug>;

	void Initialize();		//������
	void Update	   ();		//�X�V

	/*���ڂ̎��*/
	enum class ItemType
	{
		CAMERA = 0,
		PLAYER = 1,
		FPS = 2,
		ENEMY = 3,
	};

	/*getter*/
	const bool IsUseDebugMode()const;						//�f�o�b�N���[�h���g�p���邩
	const bool IsShowDebugInfo(const ItemType _type)const;	//�f�o�b�N����\�����邩
private:
	
	
	/*���������֐�*/
		 Debug			  ();//�R���X�g���N�^
		 ~Debug			  ();//�f�X�g���N�^
	void ReloadingJsonFile();//Json�t�@�C�����ēǂݍ���
	/*�����o�ϐ�*/
	std::map<int, int> jsonIndexMap;		 //���ڂ��Ƃ̊֐�
	bool			   isReloadingJsonFile;	 //Json�t�@�C�����ēǂݍ��݂�ǂݍ��񂾂��i�A���ǂݍ��݂�h���j
};

