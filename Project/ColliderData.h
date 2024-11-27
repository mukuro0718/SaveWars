//=========================================================
// �����蔻��f�[�^�N���X
//=========================================================
#pragma once
#include "GameObjectTag.h"

class Rigidbody;
class ColliderData abstract
{
public:
	/*�����蔻����*/
	enum class Kind
	{
		SPHERE				= 0,//�X�t�B�A
		CAPSULE				= 1,//�J�v�Z��
		PLANE				= 2,//�~����
		CUBE				= 3,//�L���[�u
		CHARACTER_CAPSULE	= 4,//�L�����N�^�[�J�v�Z��
		ATTACK_SPHERE		= 5,//�U���X�t�B�A
		ATTACK_CAPSULE		= 6,//�U���J�v�Z��
		MODEL = 7,
	};

	/*�ʒu�␳�̗D��x����Ɏg��*/
	enum class Priority
	{
		LOW		= 0,//��
		HIGH	= 1,//��
		STATIC	= 2,//�����Ȃ�
	};

	/*�R���X�g���N�^*/
	ColliderData(const Kind _kind, const Priority _priority, const GameObjectTag _tag);

	/*�f�X�g���N�^*/
	virtual ~ColliderData();
	
	/*�����蔻���ʎ擾*/
	Kind			GetKind			()const					 { return this->kind; }				//��ނ̎擾
	GameObjectTag	GetTag			()const					 { return this->tag; }				//�^�O���
	Priority		GetPriority		()const					 { return this->priority; }			//�D�挠�̎擾
	VECTOR			GetNextPosition	()const					 { return this->nextPositon; }		//���̍��W�̎擾
	void			SetNextPosition	(const VECTOR _position) { this->nextPositon = _position; }	//���̍��W�̃Z�b�g

	/*�����o�ϐ�*/
	Rigidbody rigidbody;//���W�b�h�{�f�B
protected:
	Kind	kind;		//���
	bool	isHit;		//����������
	VECTOR	nextPositon;//���̍��W
private:
	GameObjectTag	tag;		//�Q�[���I�u�W�F�N�g�^�O
	Priority		priority;	//�D�挠
};
