//========================================
// @brief �G�t�F�N�g�x�[�X�N���X
//========================================
#pragma once

class Transform;
class Effect abstract
{
public:
	Effect(const int _effectResourceHandle);//�R���X�g���N�^
	virtual ~Effect();//�f�X�g���N�^

		  void Initialize	 ();		//������
		  void Update		 ();		//�X�V
	const void Draw			 ()const;	//�`��
	
	/*setter*/
	void OnIsPlayEffect	() { this->isPlayEffect = true; }	//�Đ��t���O�𗧂Ă�
	void SetPosition	(const VECTOR _position);			//���W�̐ݒ�
	/*�������z�֐�*/
	virtual void SetInfoToPlayTheEffect() abstract;//�G�t�F�N�g�̍Đ��ɕK�v�ȏ��̐ݒ�
protected:
	/*�����o�ϐ�*/
	Transform*	transform;			 //�g�����X�t�H�[��
	vector<int> playingEffectHandle; //�Đ��G�t�F�N�g�n���h��
	bool		isPlayEffect;		 //�G�t�F�N�g���Đ����邩
	int			effectResourceHandle;//�G�t�F�N�g���\�[�X�n���h��
	int			frameCount;			 //�Đ��t���[���J�E���g
	int			startFrame;			 //�Đ��J�n�t���[��
	int			endFrame;			 //�Đ��I���t���[��
	int			firstFrame;			 //�ŏ��̃t���[��
};

