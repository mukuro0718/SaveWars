//===========================================
// @brief �v���C���[�ʏ�U���P
//===========================================
#pragma once

class Physics;
class Collidable;
class PlayerAttack : public GoriLib::Collidable
{
public:
	PlayerAttack();//�R���X�g���N�^
	~PlayerAttack();//�f�X�g���N�^

	void		Initialize	(GoriLib::Physics* _physics);							//������
	void		Finalize	(GoriLib::Physics* _physics);							//�㏈��
	void		Update		(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction);	//�X�V
	void		OnCollide	(const Collidable& _colider)override;					//�Փ˂����Ƃ�
	void		OnIsStart	() { this->isStartHitCheck = true; }
	const bool	GetIsStart	()const { return this->isStartHitCheck; }
	const void	Draw		()const;												//�`��
private:
	bool isStartHitCheck;
	bool isDontStartPrevFrame;
	int	 frameCount;
	int	 damage;
	float radius;
	VECTOR basePosition;
	VECTOR direction;
	//�����蔻��̃X�t�B�A�ƍ��W��Collidable�������߁A�����^�C�~���O���Ǘ�����
};

