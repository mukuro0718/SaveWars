//========================================================
// @brief �R���W�����}�l�[�W���[�N���X
// �Փ˔���֐��̎����ƊǗ����s��
// http://marupeke296.com/COL_3D_No12_OBBvsPoint.html
// ���Q�l�ɂ��܂����B
//========================================================
#pragma once
#include "Singleton.h"

class Collider;
class CollisionManager : public Singleton<CollisionManager>
{
public:
	CollisionManager();//�R���X�g���N�^
	~CollisionManager(){}//�f�X�g���N�^

	//������ĂԂ��ƂŁASingleton�N���X�Ŏ��������C���X�^���X�̎擾���ĂԂ��Ƃ��ł���
	friend class Singleton<CollisionManager>;

	const void DebugDrawHit()const;

	void HitCheck();//�����蔻��

	bool HitCheckAABBSphere(const Collider& _aabb,const Collider& _sphere);//AABB�{�b�N�X�����蔻��
	bool HitCheckOBBSphere(const Collider& _aabb, const Collider& _sphere);//OBB�{�b�N�X�����蔻��
	bool HitCheckSphereSphere(const Collider& _sphere1, const Collider& _sphere2);
	bool HitCheckSphereCapsule(const Collider& _sphere, const Collider& _capsule);
private:
	/*���������֐�*/
	float GetLengthAABBToPoint(const Collider& _aabb, const Collider& _point);//AABB�I�u�W�F�N�g��1�_�̍ŋߐړ_�����߂�
	float GetLengthOBBToPoint(const Collider& _aabb, const Collider& _point);//OBB�I�u�W�F�N�g��1�_�̍ŋߐړ_�����߂�


	/*�����o�ϐ�*/
	bool isHit;//�f�o�b�N���ɓ����������ǂ�����\���i�{���͎g�p���Ȃ��j
	VECTOR fixVector;
	VECTOR debugMax;
	VECTOR debugMin;
};

