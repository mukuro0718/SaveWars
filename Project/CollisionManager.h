//===============================================================
// @brief �����A�Փ˔��肷��I�u�W�F�N�g��o�^���A
// �����ړ��A�Փ˂�ʒm����
//===============================================================
#pragma once
#include "Singleton.h"

class ColliderData;
class CollisionManager : public Singleton<CollisionManager>
{
public:
	/*Singleton�N���X��ColliderManager�ɃA�N�Z�X�ł���悤�ɂ���*/
	friend class Singleton<CollisionManager>;

	virtual ~CollisionManager(){}//�f�X�g���N�^


	void Entry	(ColliderData& _data);	//�Փ˕��̓o�^
	void Exit	(ColliderData& _data);	//�o�^����
	void Update	();						//�X�V(�o�^�I�u�W�F�N�g�̕����ړ��A�Փ˒ʒm)
private:
	/*�ÓI�萔*/
	static constexpr float	GRAVITY					= -0.098f;	//�d��
	static constexpr float	MAX_GRAVITY_ACCEL		= -1.0f;	//�ő�d�͉����x
	static constexpr int	BEFORE_FIX_INFO_COLOR	= 0x0000ff;	//�␳�O���F
	static constexpr int	AIM_INFO_COLOR			= 0x0000aa;	//�␳�O�\����F
	static constexpr int	AFTER_FIX_INFO_COLOR	= 0xff00ff;	//�␳����F
	static constexpr int	DIV_NUM					= 16;		//�f�o�b�N�\���}�`������
	static constexpr int	MAX_CHECK_COUNT			= 100;		//�����蔻����m�F����ő��

	/*���������֐�*/
	CollisionManager	(){}												//�R���X�g���N�^
	void CheckColide	();													//�����蔻��`�F�b�N
	bool IsCollide		(ColliderData& _objectA, ColliderData& _objectB);	//����������
	void FixNextPosition(ColliderData& _primary, ColliderData& _secondary);	//���̍��W��␳
	void FixPosition	();													//���W�m��

	/*�����o�ϐ�*/
	std::list<ColliderData*> collidables;//�o�^���ꂽCollidable�̃��X�g
};

