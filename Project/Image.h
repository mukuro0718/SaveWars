#pragma once
class Image
{
public:
	 Image(const int _imageHandle);//�R���X�g���N�^
	~Image(){}//�f�X�g���N�^

	/*�`��*/
	const void Draw			()const;//�ʏ�`��
	const void DrawExtend	()const;//�g�債�ĕ`��
	const void DrawBillboard()const;//�RD��Ԃɕ`��

	/*�\����*/
	//�ʏ�`��Ɏg�p����\����
	struct Normal
	{
		int x, y;
	};
	//�g��`��Ɏg�p����\����
	struct Extend
	{
		int leftX, leftY, rightX, rightY;
	};
	//�RD��ԕ`��Ɏg�p����\����
	struct Billboard
	{
		VECTOR position;
		float centerX, centerY;
		float size;
		float angle;
	};

	/*getter*/
	const Normal	GetNormal	()const { return this->normal; }	//�ʏ�
	const Extend	GetExtend	()const { return this->extend; }	//�g��
	const Billboard GetBillboard()const { return this->billboard; }	//�RD���
	
	/*setter*/
	void SetNormal	 (const Normal _normal)		  { this->normal = _normal; }		//�ʏ�
	void SetExtend	 (const Extend _extend)		  { this->extend = _extend; }		//�g��
	void SetBillboard(const Billboard _billboard) { this->billboard = _billboard; }	//�RD���	
private:
	/*�ÓI�萔*/
	static constexpr int MAX_ALPHA = 255;
	static constexpr int MIN_ALPHA = 0;

	/*�����o�ϐ�*/
	Normal		normal;
	Extend		extend;
	Billboard	billboard;
	int			imageHandle;
	int			alpha;
};

