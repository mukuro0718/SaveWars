#pragma once

class Character;
class Rigidbody;
class BossAction;
class BossDeath : public BossAction
{
public:
			 BossDeath();//�R���X�g���N�^
	virtual ~BossDeath();//�f�X�g���N�^

	void		Initialize	 ()																override;//������
	void		Finalize	 ()																override;//�㏈��
	Rigidbody&	Update		 (Character& _boss)												override;//�X�V
	void		CalcParameter(const int _hp, const int _angryValue, const float _distance)	override;//�p�����[�^�̌v�Z
private:

};

