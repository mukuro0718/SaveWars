//======================================================
// @brief �ϋq�N���X
//======================================================

#pragma once
class Audience
{
public:
	Audience();		//�R���X�g���N�^
	~Audience();		//�f�X�g���N�^
	void Initialize();//������
	void Update();
	void Draw();	//�`��
private:
	/*�ÓI�萔*/
	static constexpr int VERTEX_NUM = 4;
	static constexpr int INDEX_NUM = 6;
	static constexpr int POLYGON_NUM = 2;

	/*���������֐�*/
	int		ConvertColor(const std::vector<int> _color);
	VECTOR	Convert		(const std::vector<int> _in);
	/*�����o�ϐ�*/
	VERTEX3D vertex[VERTEX_NUM];
	WORD index[INDEX_NUM];
	int imageHandle;
};
