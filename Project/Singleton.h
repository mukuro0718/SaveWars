//===========================================================
// @brief�@�V���O���g���e���v���[�g�N���X
// �Q�l:https://qiita.com/narumi_/items/b205e59e7fc81695f380
//===========================================================
#pragma once
template <typename T>
class Singleton
{
public:
	/*�C���X�^���X�̎擾*/
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
protected:
	Singleton() { }
	virtual ~Singleton(){}

private:
	void operator=(const Singleton& _object){}//������Z�q�֎~
	Singleton(const Singleton& _object){}//�R�s�[�R���X�g���N�^�֎~
};

//#pragma once
//template <typename T>
//class Singleton
//{
//public:
//	/*�C���X�^���X�̍쐬*/
//	static void CreateInstance()
//	{
//		instance = new T();
//	}
//
//	/*�C���X�^���X�̍폜*/
//	static void DeleteInstance()
//	{
//		if (instance != nullptr)
//		{
//			delete(instance);
//			instance = nullptr;
//		}
//	}
//
//	/*�C���X�^���X���擾*/
//	static T& GetInstance()
//	{
//		return *instance;
//	}
//
//protected:
//	/*���������֐�*/
//	Singleton() { }
//	virtual ~Singleton() {}
//	/*�����o�ϐ�*/
//	static T* instance;
//};

