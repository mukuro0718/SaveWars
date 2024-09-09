//=============================================================================================
// @brief �V�[���n��UI�N���X
// �^�C�g�����S�A�Q�[���I�[�o�[/�N���A
// ���U���g�̕`����s��
//=============================================================================================
#pragma once


class SceneUI abstract
{
public:
			 SceneUI(){}//�R���X�g���N�^
	virtual ~SceneUI(){}//�f�X�g���N�^

	virtual void		Initialize	() abstract;	  //������
	virtual	void		Update		() abstract;	  //�X�V
	virtual const void	Draw		() const abstract;//�`��
protected:
	/*�\����*/
	struct Image
	{
	public:
		static constexpr int MAX_ALPHA = 255;//�ő�A���t�@
		int lx, ly, rx, ry;					 //���W
		int imageHandle;					 //�摜�n���h��
		int alpha;							 //�A���t�@�l
		bool isAddAlpha;
		const void Draw()const;
		void SetPosition(std::vector<int> _position);
		void AddAlpha(const int _addAlpha);
		void DecreaseAlpha(const int _decreaseAlpha);
		void FlashingAlpha(const int _addAlpha);
	};
};

