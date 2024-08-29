//===============================================================
// @brief �����A�Փ˔��肷��I�u�W�F�N�g��o�^���A
// �����ړ��A�Փ˂�ʒm����
//===============================================================
#pragma once
#include <list>
#include <vector>

namespace GoriLib
{
	class Collidable;
	class Physics final
	{
	public:
		void Entry(Collidable* collidable);//�Փ˕��̓o�^
		void Exit(Collidable* collidable);//�o�^����
		void Update();//�X�V(�o�^�I�u�W�F�N�g�̕����ړ��A�Փ˒ʒm)

		//�w�胉�C�����I�u�W�F�N�g�ƂԂ����Ă��邩�ǂ����𔻒肵�A
		//�Ԃ����Ă���I�u�W�F�N�g��Ԃ�
		std::list<Collidable*> IsCollideLine(const VECTOR& _start, const VECTOR& _end)const;

		//�d�͂ƍő�d�͉����x
		static constexpr float GRAVITY = -0.01f;
		static constexpr float MAX_GRAVITY_ACCEL = -0.15f;
	private:
		static constexpr int BEFORE_FIX_INFO_COLOR = 0x0000ff;//�␳�O���F
		static constexpr int AIM_INFO_COLOR = 0x0000aa;//�␳�O�\����F
		static constexpr int AFTER_FIX_INFO_COLOR = 0xff00ff;//�␳����F
		static constexpr int DIV_NUM = 16;
		static constexpr int MAX_CHECK_COUNT = 1000;

		std::list<Collidable*> collidables;//�o�^���ꂽCollidable�̃��X�g

		//OnCollid�̒x���ʒm�̂��߂̃f�[�^
		struct OnCollideInfo
		{
			Collidable* owner;
			Collidable* colider;
			void OnCollide() { owner->OnCollide(*colider); }
		};

		//�����蔻��`�F�b�N
		std::vector<OnCollideInfo> CheckColide()const;
		bool IsCollide(const Collidable* _objectA, const Collidable* _objectB)const;

		//�ʒu�␳�A����
		void FixNextPosition(Collidable* _primary, Collidable* _secondary)const;
		void FixPosition();
	};
}
