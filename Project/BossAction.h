//================================================
// @brief �{�X�A�N�V�����̊��N���X
//================================================
#pragma once

class Boss;
class ActionParameter;
class BossAction abstract
{
public:
			 BossAction();	//�R���X�g���N�^
	virtual ~BossAction(){}	//�f�X�g���N�^

	virtual void Update		  (Boss& _boss)		abstract;//�X�V
	virtual void CalcParameter(const Boss& _boss)	abstract;//�p�����[�^�[�̌v�Z

	/*getter/setter*/
	const int  GetDesireValue()	const;
	const bool GetIsSelect	 ()	const { return this->isSelect; }
		  void OnIsSelect	 () { this->isSelect = true; }
	const int  GetWeight	 (const int _sum)const;
	const int GetBaseWeight	 ()const;
protected:
	/*���������֐�*/
	bool FrameCount(const int _maxFrame);
	void OffIsSelect(const int _maxInterval);

	/*�����o�ϐ�*/
	ActionParameter* parameter;		//�p�����[�^
	bool			 isSelect;		//�A�N�V�������I�����ꂽ��
	int				 frameCount;	//�t���[���J�E���g
	bool			 isInitialize;	//���������ꂽ��
};

