//=============================================
// @brief �{�X��]�X���b�V���U���A�N�V����
//=============================================
#pragma once

class Boss;
class BossAction;
class BossRotateSlashAction : public BossAction
{
public:
	 BossRotateSlashAction();//�R���X�g���N�^
	~BossRotateSlashAction();//�f�X�g���N�^

	void Initialize		()					override;//������
	void Update			(Boss& _boss)		override;//�X�V
	void CalcParameter	(const Boss& _boss)	override;//�p�����[�^�[�̌v�Z
private:
	bool isClose;//�߂Â�����
};

