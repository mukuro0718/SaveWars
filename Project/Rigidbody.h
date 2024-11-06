//=========================================================
// @brief �����A�Փ˔���ɕK�v�ȃf�[�^
//=========================================================
#pragma once

class Rigidbody
{
public:
	Rigidbody();//�R���X�g���N�^
	void Initialize(bool _useGravity = false);//������
	/*getter/setter*/
	const	VECTOR& GetPosition	()const { return this->position;	}//���W�̎擾
	const	VECTOR& GetRotation	()const { return this->rotation;	}//��]���̎擾
	const	VECTOR& GetScale	()const { return this->scale;		}//�g�嗦�̎擾
	const	VECTOR& GetDirection()const { return this->direction;	}//�����̎擾
	const	VECTOR& GetVelocity	()const { return this->velocity;	}//Velocity�̎擾
			bool	UseGravity	()const { return this->useGravity;	}//�d�͂̎擾
	void SetPosition	(const VECTOR& _set) { this->position = _set;	}//���W�̐ݒ�
	void SetRotation	(const VECTOR& _set) { this->rotation = _set;	}//��]���̐ݒ�
	void SetScale		(const VECTOR& _set) { this->scale = _set;		}//�g�嗦�̐ݒ�
	void SetUseGravity	(bool _set)			 { this->useGravity = _set; }//�d�͂̐ݒ�
	void SetVelocity	(const VECTOR& _set);							 //Velocity�̐ݒ�
private:
	VECTOR	position;	//���W
	VECTOR  rotation;	//��]��
	VECTOR  scale;		//�g�嗦
	VECTOR	direction;	//����
	VECTOR	velocity;	//�ړ���
	bool	useGravity;	//�d�͂��g�p���邩
};
