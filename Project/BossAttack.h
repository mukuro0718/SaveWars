//===========================================
// @brief �{�X�U��
//===========================================
#pragma once

class Physics;
class Collidable;
class BossAttack : public GoriLib::Collidable
{
public:
	BossAttack(const int _attackNum);//�R���X�g���N�^
	~BossAttack();//�f�X�g���N�^

	void		Initialize(GoriLib::Physics* _physics, const float _radius);							//������
	void		Finalize(GoriLib::Physics* _physics);							//�㏈��
	void		Update(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction, const bool _isMove, const float _speed);	//�X�V
	void		OnCollide(const Collidable& _colider)override;					//�Փ˂����Ƃ�
	void		OnIsStart() { this->isStartHitCheck = true; }
	const void	Draw()const;												//�`��
	const VECTOR GetPosition()const;
private:
	bool isStartHitCheck;
	bool isDontStartPrevFrame;
	int	 frameCount;
	int	 damage;
	float radius;
	VECTOR basePosition;
	VECTOR direction;
	int attackNum;
	float stackSpeed;
	//�����蔻��̃X�t�B�A�ƍ��W��Collidable�������߁A�����^�C�~���O���Ǘ�����
};

