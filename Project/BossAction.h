//================================================
// @brief �{�X�A�N�V�����̊��N���X
//================================================
#pragma once

class Character;
class Rigidbody;
class ActionParameter;
class BossAction abstract
{
public:
			 BossAction();	//�R���X�g���N�^
	virtual ~BossAction(){}	//�f�X�g���N�^

	virtual void		Initialize	 ()																abstract;//������
	virtual void		Finalize	 ()																abstract;//�㏈��
	virtual Rigidbody&	Update		 (Character& _boss)												abstract;//�X�V
	virtual void		CalcParameter(const int _hp, const int _angryValue, const float _distance)	abstract;//�p�����[�^�̌v�Z

	/*getter*/
	const int GetDesireValue() const;
protected:
	ActionParameter* parameter;//�p�����[�^
};

