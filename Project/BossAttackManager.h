#pragma once

class Physics;
class Collidable;
namespace GoriLib
{
	class BossAttack;
}
class BossAttackManager
{
public:
	BossAttackManager();//�R���X�g���N�^
	~BossAttackManager();//�f�X�g���N�^

	void		Initialize(GoriLib::Physics* _physics);		//������
	void		Finalize(GoriLib::Physics* _physics);		//�㏈��
	void		Update(GoriLib::Physics* _physics);		//�X�V
	void		OnCollide(const GoriLib::Collidable& _colider);//�Փ˂����Ƃ�
	const void	Draw()const;	//�`��
	void		OnIsStart(const int _index);
	const VECTOR GetPosition(const int _index);
	const VECTOR GetThrowPosition();
private:
	enum class AttackType
	{
		NONE		   = -1,
		SLASH		   = 0,//�X���b�V��
		FLY_ATTACK	   = 1,//��эU��
		HURRICANE_KICK = 2,//��]�R��
		JUMP_ATTACK	   = 3,//�W�����v�A�^�b�N
		ROTATE_PUNCH   = 4,//��]�p���`
	};
	std::vector<GoriLib::BossAttack*> attack;
	int hitNumber;
};

