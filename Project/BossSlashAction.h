//=============================================
// @brief �{�X�X���b�V���U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossSlashAction : public BossAction
{
public:
	 BossSlashAction();//�R���X�g���N�^
	~BossSlashAction();//�f�X�g���N�^

	void Initialize		()					override;//������
	void Update			(Boss& _boss)		override;//�X�V
	void CalcParameter	(const Boss& _boss) override;//�p�����[�^�[�̌v�Z
private:
	bool isClose;//�߂Â�����
};

