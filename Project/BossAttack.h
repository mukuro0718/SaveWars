//===========================================
// @brief �{�X�U��
//===========================================
#pragma once

namespace GoriLib
{
	class Physics;
	class Collidable;
	class ColliderData;
	class BossAttack abstract : public GoriLib::Collidable
	{
	public:
		BossAttack(Priority _priority, GameObjectTag _tag, ColliderData::Kind _colliderKind, bool _isTrigger);//�R���X�g���N�^
		~BossAttack();//�f�X�g���N�^

		virtual void	Initialize(GoriLib::Physics* _physics) abstract;//������
		virtual void	Finalize(GoriLib::Physics* _physics);//�㏈��
		virtual void	Update(GoriLib::Physics* _physics) abstract;//�X�V
		virtual void	OnCollide(const Collidable& _colider) override;//�Փ˂����Ƃ�
		virtual const void	Draw()const						 abstract;//�`��
		void	OnIsStart() { this->isStartHitCheck = true; }	  //�����蔻��J�n�t���O�𗧂Ă�
		const	VECTOR	GetPosition()const;							  //���W�̎擾
	protected:
		/*���������֐�*/
		virtual void		SetRadius(const float _radius) abstract;//���a�̃Z�b�g
		virtual const float GetRadius() const			   abstract;//���a�̎擾
		const VECTOR Convert(std::vector<float> _in);

		/*�����o�ϐ�*/
		int		frameCount;			 //�t���[���J�E���g
		int		damage;				 //�_���[�W
		int		attackIndex;		 //�U���ԍ�
		float	stackSpeed;			 //�~�ς����X�s�[�h
		VECTOR	basePosition;		 //���̍��W
		VECTOR	direction;			 //����
		bool	isStartHitCheck;	 //�����蔻����s����
		bool	isDontStartPrevFrame;//�O�t���[���œ����蔻�肪�s���Ă�����
		//�����蔻��̃X�t�B�A�ƍ��W��Collidable�������߁A�����^�C�~���O���Ǘ�����
	};
}
