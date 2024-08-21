//===========================================================
// @brief �v���C���[���@�N���X
//===========================================================
#pragma once
#include "Singleton.h"

class PlayerMagic;
class PlayerMagicManager :public Singleton<PlayerMagicManager>
{
public:
	friend class Singleton<PlayerMagicManager>;

	void Initialize();		//������
	void Update();			//�X�V
	const void Draw()const;	//�`��
private:
	/*�ÓI�萔*/
	static constexpr int MAGIC_NUM = 5;

	/*�񋓑�*/
	enum class Element
	{
		FRAME,
		BOLT,
		ICE,
		EARTH,
		STORM,
	};

	/*���������֐�*/
	PlayerMagicManager();//�R���X�g���N�^
	~PlayerMagicManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	std::array<PlayerMagic* ,MAGIC_NUM> magic;
};

