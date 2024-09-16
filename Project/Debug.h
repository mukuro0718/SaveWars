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
	const void Draw()const;	//�`��

	/*getter*/
	const bool CheckFPSFlag()const;
	const bool CheckPlayerFlag()const;
	const bool CheckEnemyFlag()const;
	const bool CheckCameraFlag()const;//�J�����̃f�o�b�N�@�\��ON�ɂȂ��Ă��邩�ǂ���
private:
	/*�t�@���N�V����*/
	typedef std::function<void(void)> FlagsState;//�t���O�̐؂�ւ����s��
	
	/*�N���X�E�\����*/
	//�t���O�̐؂�ւ����s���֐��Q
	class FlagsStateSet
	{
	public:
		FlagsState set;
		FlagsState clear;
	};
	//���ڂ̎��
	enum class ItemType
	{
		CAMERA	= 0,
		PLAYER	= 1,
		FPS		= 2,
		ENEMY	= 3,
	};

	/*�ÓI�萔*/
	static const int COLOR_WHITE;//���F
	static const int COLOR_BLACK;//���F
	static constexpr unsigned int CAMERA		 = (1 << 0);//�J����
	static constexpr unsigned int PLAYER		 = (1 << 1);//�v���C���[
	static constexpr unsigned int FPS			 = (1 << 2);//FPS
	static constexpr unsigned int ENEMY			 = (1 << 3);//�G�l�~�[
	static constexpr unsigned int MASK_ALL_FLAGS = CAMERA | PLAYER | FPS | ENEMY;
	
	/*���������֐�*/
			   Debug();																			 //�R���X�g���N�^
			   ~Debug();																		 //�f�X�g���N�^
		  void ChangeSelectedItem();															 //�I�����ڂ̕ύX
		  void ChangeFlagsState	 ();															 //�t���O�̏�Ԃ̕ύX
		  void AddItemFunction	 (const int _item,const FlagsState _set,const FlagsState _clear);//���ڂ��Ƃ̊֐��̒ǉ�
	const bool IsShow			 ()const;														 //�f�o�b�N���ڂ�\�������邩
	const unsigned int GetBitFlagInMap(const int _itemName)const;
	/*�����o�ϐ�*/
	std::map<int, FlagsStateSet> itemFunctionMap;		//���ڂ��Ƃ̊֐�
	BitFlag*					 flags;//�t���O�i�f�o�b�N�@�\�̎g�p�󋵁j
	bool						 isShow;				//�f�o�b�N���ڂ�\�������邩
	int							 previousSelectedItem;	//�ȑO�I������Ă�������
	int							 currentlySelectedItem;	//���I������Ă��鍀��
};

