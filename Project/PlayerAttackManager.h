#pragma once

class Physics;
class Collidable;
class PlayerMain_1Attack;
class PlayerMain_2Attack;
class PlayerSpecialAttack;
class PlayerAttackManager
{
public:
	PlayerAttackManager();//�R���X�g���N�^
	~PlayerAttackManager();//�f�X�g���N�^

	void		Initialize(GoriLib::Physics* _physics);		//������
	void		Finalize(GoriLib::Physics* _physics);		//�㏈��
	void		Update(GoriLib::Physics* _physics);		//�X�V
	void		OnCollide(const GoriLib::Collidable& _colider);//�Փ˂����Ƃ�
	const void	Draw()const;	//�`��
	void		OnIsStart(const int _index);
private:
	enum class AttackType
	{
		NONE = -1,
		MAIN_1 = 0,//�r��
		MAIN_2 = 1,//�r��
		SPECIAL = 2,//�R���{
	};
	PlayerMain_1Attack* main1;
	PlayerMain_2Attack* main2;
	PlayerSpecialAttack* special;
	int hitNumber;
};

