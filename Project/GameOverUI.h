//===============================================
// @brief �N�G�X�g���s����UI
//===============================================
#pragma once

class GameOverUI
{
public:
	 GameOverUI();//�R���X�g���N�^
	~GameOverUI();//�f�X�g���N�^

	void		Initialize();	//������
	void		Update();		//�X�V
	const void	Draw() const;	//�`��

	const bool GetIsProductFinished()const { return this->isProductFinished; }
private:
	/*�񋓑�*/
	//���o�̒i�K
	enum class ProductStage
	{
		WAIT_FOR_FADE_IN,
		LOGO_FADE_IN,
		WAIT_FOR_FADE_OUT,
		LOGO_FADE_OUT,
	};

	/*�萔*/
	static constexpr int	MAX_ALPHA	= 255;						//�ő�A���t�@�l
	static constexpr double LOGO_ANGLE	= 0.0;						//���S�̍ŏ��̉�]��
	const float				FIRST_LOGO_RATE;						//���S�̍ŏ��̑傫��
	const float				TARGET_LOGO_RATE_FOR_FADE_IN;			//�t�F�[�h�C�����̃��S�̖ڕW�̑傫��
	const float				LERP_VALUE_FOR_REDUCTION;				//�k���p���[�v�l
	const int				WAIT_TIME_BEFORE_START_FADE_IN;			//�t�F�[�h�C���J�n�O�̑ҋ@����
	const int				WAIT_TIME_BEFORE_START_FADE_OUT;		//�t�F�[�h�A�E�g�J�n�O�̑ҋ@����
	const int				INCREASED_ALPHA_FOR_LOGO_FADE_IN;		//���S�t�F�[�h�C���p�A���t�@�l������
	const int				REDUCTION_ALPHA_FOR_LOGO_FADE_OUT;		//���S�t�F�[�h�A�E�g�p�A���t�@�l������
	const int				POSITION_X;								//�`����W
	const int				FIRST_POSITION_Y;						//�`����W
	const int				POSITION_Y_FOR_FADE_OUT_MAGNIFICATION;	//�t�F�[�h�A�E�g����Y���W������

	/*�����o�ϐ�*/
	ProductStage currentProductStage;	//���݂̉��o�̒i�K
	int			 logoAlpha;				//���S�̃A���t�@�l
	float		 logoRate;				//���S�̑傫��
	int			 waitTimer;				//�ҋ@����(�t���[�����Ƃ�-1�����)
	bool		 isProductFinished;		//���o���I��������
	int			 questFailedLogo;		//�N�G�X�g���s���̃��S
	int			 positionY;				//�`����W
};

