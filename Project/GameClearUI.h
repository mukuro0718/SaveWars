//===============================================
// @brief �N�G�X�g�N���A����UI
//===============================================
#pragma once

class GameClearUI
{
public:
	 GameClearUI();//�R���X�g���N�^
	~GameClearUI();//�f�X�g���N�^

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
		EFFECT_FADE_OUT,
		WAIT_FOR_FADE_OUT,
		LOGO_FADE_OUT,
	};

	/*�萔*/
	static constexpr int DRAW_POSITION_X = 960;							//�`����W
	static constexpr int DRAW_POSITION_Y = 540;							//�`����W
	static constexpr int MAX_ALPHA		 = 255;								//�ő�A���t�@�l
	const float			 FIRST_LOGO_ANGLE;								//���S�̍ŏ��̉�]��
	const float			 TARGET_LOGO_ANGLE;								//���S�̉�]�ڕW
	const float			 FIRST_LOGO_RATE;								//���S�̍ŏ��̑傫��
	const float			 TARGET_LOGO_RATE_FOR_FADE_IN;					//�t�F�[�h�C�����̃��S�̖ڕW�̑傫��
	const float			 TARGET_LOGO_RATE_FOR_FADE_OUT;					//�t�F�[�h�A�E�g���̃��S�̖ڕW�̑傫��
	const float			 TARGET_EFFECT_LOGO_RATE;						//�G�t�F�N�g�p���S�̖ڕW�̑傫��
	const float			 LERP_VALUE_FOR_ANGLE;							//�A���O���p���[�v�l
	const float			 LERP_VALUE_FOR_REDUCTION;						//�k���p���[�v�l
	const float			 LERP_VALUE_FOR_FADE_OUT_MAGNIFICATION;			//�t�F�[�h�C�����g��p���[�v�l
	const float			 LERP_VALUE_FOR_EFFECT_FADE_OUT_MAGNIFICATION;	//�t�F�[�h�C�����G�t�F�N�g�g��p���[�v�l
	const int			 WAIT_TIME_BEFORE_START_FADE_IN;				//�t�F�[�h�C���J�n�O�̑ҋ@����
	const int			 WAIT_TIME_BEFORE_START_FADE_OUT;				//�t�F�[�h�A�E�g�J�n�O�̑ҋ@����
	const int			 INCREASED_ALPHA_FOR_LOGO_FADE_IN;				//���S�t�F�[�h�C���p�A���t�@�l������
	const int			 REDUCTION_ALPHA_FOR_LOGO_FADE_OUT;				//���S�t�F�[�h�A�E�g�p�A���t�@�l������
	const int			 REDUCTION_ALPHA_FOR_EFFECT_FADE_OUT;			//�G�t�F�N�g�t�F�[�h�A�E�g�p�A���t�@�l������

	/*�����o�ϐ�*/
	ProductStage currentProductStage;	//���݂̉��o�̒i�K
	float		 logoAngle;				//���S�̃A���O��
	float		 logoRate;				//���S�̑傫��
	int			 logoAlpha;				//���S�̃A���t�@�l
	float		 effectLogoRate;		//�G�t�F�N�g�p�̃��S�̑傫��
	int			 effectLogoAlpha;		//�G�t�F�N�g�̃A���t�@�l
	int			 waitTimer;				//�ҋ@����(�t���[�����Ƃ�-1�����)
	bool		 isProductFinished;		//���o���I��������
	int			 questClearLogo;		//�N�G�X�g�N���A���̃��S
};

