#include <DxLib.h>
#include <iostream>
#include <fstream>
#include "UseSTL.h"
#include "json.hpp"
#include "JsonLoader.h"

// for convenience
using json = nlohmann::json;
//JsonLoader* Singleton<JsonLoader>::instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
JsonLoader::JsonLoader()
{
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
JsonLoader::~JsonLoader()
{

}

/// <summary>
/// �w�肳�ꂽjson�t�@�C����ǂݍ��݃f�[�^��Ԃ�
/// </summary>
const json JsonLoader::GetJsonData(std::string _jsonName)const
{
    json jsonData;
    /*�t�@�C����ǂݍ���œ��e����ʂɕ\������*/
    std::ifstream ifs(_jsonName.c_str());
    if (ifs.good())
    {
        ifs >> jsonData;
    }
    /*�ǂݍ��݂Ɏ��s�����ꍇ�̓G���[��\������*/
    else
    {
        std::cout << "�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B" << std::endl;
    }
    return jsonData;
}
