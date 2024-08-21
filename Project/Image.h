#pragma once
class Image
{
public:
	 Image(const int _imageHandle);//コンストラクタ
	~Image(){}//デストラクタ

	/*描画*/
	const void Draw			()const;//通常描画
	const void DrawExtend	()const;//拡大して描画
	const void DrawBillboard()const;//３D空間に描画

	/*構造体*/
	//通常描画に使用する構造体
	struct Normal
	{
		int x, y;
	};
	//拡大描画に使用する構造体
	struct Extend
	{
		int leftX, leftY, rightX, rightY;
	};
	//３D空間描画に使用する構造体
	struct Billboard
	{
		VECTOR position;
		float centerX, centerY;
		float size;
		float angle;
	};

	/*getter*/
	const Normal	GetNormal	()const { return this->normal; }	//通常
	const Extend	GetExtend	()const { return this->extend; }	//拡大
	const Billboard GetBillboard()const { return this->billboard; }	//３D空間
	
	/*setter*/
	void SetNormal	 (const Normal _normal)		  { this->normal = _normal; }		//通常
	void SetExtend	 (const Extend _extend)		  { this->extend = _extend; }		//拡大
	void SetBillboard(const Billboard _billboard) { this->billboard = _billboard; }	//３D空間	
private:
	/*静的定数*/
	static constexpr int MAX_ALPHA = 255;
	static constexpr int MIN_ALPHA = 0;

	/*メンバ変数*/
	Normal		normal;
	Extend		extend;
	Billboard	billboard;
	int			imageHandle;
	int			alpha;
};

