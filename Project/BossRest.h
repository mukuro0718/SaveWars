//==========================================================
// @brief �{�X�x�e�A�N�V�����N���X
// �������Ȃ�
//==========================================================
#pragma once

class Character;
class Rigidbody;
class BossAction;
class BossRest : public BossAction
{
public:
	BossRest();//�R���X�g���N�^
	virtual ~BossRest();//�f�X�g���N�^

	void		Initialize()																override;//������
	void		Finalize()																override;//�㏈��
	Rigidbody& Update(Character& _boss)												override;//�X�V
	void		CalcParameter(const int _hp, const int _angryValue, const float _distance)	override;//�p�����[�^�̌v�Z
private:

};

