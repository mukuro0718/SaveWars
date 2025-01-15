//===============================================
// @brief �Z���N�g�V�[��UI�N���X
//===============================================
#pragma once

class Image;
class SceneUI;
class SelectUI : public SceneUI
{
public:
	 SelectUI();//�R���X�g���N�^
	~SelectUI();//�f�X�g���N�^

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
private:
	/*�����o�ϐ�*/
	vector<int>	icon;			//�摜�n���h��
	vector<int>	summary;		//�摜�n���h��
	vector<int> image;			//�摜�n���h��
	int			background;		//�w�i
	int			logo;			//���S
	int			iconFrame;		//�A�C�R���̃t���[��
	int			provDecide;		//����A�C�R��
	int			nowSelectEnemy;	//���ݑI������Ă���G
	bool		isEnd;			//�I�����邩
};

