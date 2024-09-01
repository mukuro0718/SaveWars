#pragma once

class Physics;
class Collidable;
class BossAttack;
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
	static constexpr int ATTACK_NUM = 3;
	enum class AttackType
	{
		NONE = -1,
		SLASH = 0,//�r��
		ROTATE_PUNCH = 1,//�r��
		JUMP_ATTACK = 2,//�R���{
	};
	std::vector<BossAttack*> attack;
	int hitNumber;
};

