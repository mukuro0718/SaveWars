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
	int background;
	int logo;
	int iconFrame;
	int provDecide;
	vector<int>	icon;//�摜�n���h��
	vector<int>	summary;//�摜�n���h��
	vector<int> image;

	int nowSelectEnemy;//���ݑI������Ă���G
	bool		isEnd;				//�I�����邩
};

