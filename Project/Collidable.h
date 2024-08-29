//==========================================================
// @brief �Փ˂ł���\���̂�����̂Ɍp��
//==========================================================
#pragma once
#include <list>
#include "GameObjectTag.h"

namespace GoriLib
{
	class Rigidbody;
	class Physics;
	class ColliderData;
	class Collidable abstract
	{
	public:
		/*�ʒu�␳�̗D��x����Ɏg��*/
		enum class Priority
		{
			LOW,	//��
			HIGH,	//��
			STATIC,//�����Ȃ�
		};

		Collidable(Priority _priority, GameObjectTag _tag, ColliderData::Kind _colliderKind, bool _isTrigger);//�R���X�g���N�^
		virtual ~Collidable();//�f�X�g���N�^
		virtual void Initialize(GoriLib::Physics* _physics);
		virtual void Finalize(GoriLib::Physics* _physics);

		virtual void OnCollide(const Collidable& _colider) abstract;//�Փ˂����Ƃ�

		GameObjectTag GetTag()const { return tag; }//�^�O���
		Priority GetPriority()const { return priority; }

		//�����蔻��𖳎�����^�O�̒ǉ�/�폜
		void AddThroughTag(GameObjectTag _tag);
		void RemoveThroughTag(GameObjectTag _tag);

		//�����蔻��𖳎�����Ώۂ��ǂ���
		bool IsThroughTarget(const Collidable* _target)const;

	protected:
		Rigidbody rigidbody;//�����f�[�^
		ColliderData* colliderData;//�����蔻��f�[�^
	private:
		ColliderData* CreateColliderData(ColliderData::Kind _kind, bool _isTrigger);

		GameObjectTag tag;
		Priority priority;
		std::list<GameObjectTag> throughTags;//�����蔻��𖳎����邽�߂̃��X�g
		
		
		friend Physics;//Physics��Collidable�����R�ɊǗ����邽�߂Ƀt�����h��
		/*�ȉ��t�����h�ł���Physics�݂̂������^��ϐ�*/
	private:
		//�����Physics�����ق����]�܂���
		VECTOR nextPosition;

	};
}
