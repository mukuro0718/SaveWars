//===================================================
// @brief �摜�N���X
//===================================================
#pragma once
class Image final
{
public:
	 Image(int _imageHandle);//�R���X�g���N�^
	~Image(){}//�f�X�g���N�^

	const void	Draw()const;						//�`��
	void		SetPosition(std::vector<int> _position);	//���W�̐ݒ�

	bool ExtendGraph(const std::vector<int> _targetPosition, const std::vector<int> _addValue);
	void ScalingGraph(const std::vector<int> _extendPosition, const std::vector<int> _reducedPosition, const std::vector<int> _scalingValue);
	void		FadeIn(const int _addAlpha);//�t�F�[�h�C��
	void		FadeOut(const int _addAlpha);//�t�F�[�h�A�E�g
	void		FadeInOut(const int _addAlpha);//�t�F�[�h�C���A�E�g
	
	/*�����o�ϐ�*/
	static constexpr int MAX_ALPHA = 255;//�ő�A���t�@
	vector<int>  position;//���W
	int  alpha;			//�A���t�@�l
	bool isAddAlpha;	//�A���t�@��ǉ����邩�ifalse	�Ȃ猸���̂݋������j
	int interval;		//�C���^�[�o��
	int  imageHandle;	//�摜�n���h��
	bool isExtend;
};

