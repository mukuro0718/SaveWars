//================================================================
// @brief json�ǂݍ��݃N���X
// �w�肳�ꂽjson�t�@�C����ǂݍ���
//================================================================
#pragma once
#include "Singleton.h"

class JsonLoader : public Singleton<JsonLoader>
{
public:
	friend class Singleton<JsonLoader>;
	
	/*�w�肳�ꂽJson�t�@�C����ǂݍ��݃f�[�^��Ԃ�*/
	const nlohmann::json GetJsonData(std::string _jsonName)const;
private:
	/*���������֐�*/
			 JsonLoader();//�R���X�g���N�^
	virtual ~JsonLoader();//�f�X�g���N�^
};

