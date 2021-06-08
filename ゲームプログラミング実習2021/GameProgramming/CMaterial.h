#ifndef CMATERIAL_H
#define CMATERIAL_H
#include "CTexture.h"
#include"CModel.h"
/*
�}�e���A���N���X
�}�e���A���̃f�[�^������
*/
class CMaterial {
public:
	//�}�e���A�����̒��_��
	int mVertexNum;
	//�e�N�X�`��
	CTexture mTexture;
	//�}�e���A����
	char mName[64];
	//�g�U���̐FRGBA
	float mDiffuse[4];
	//�f�t�H���g�R���X�g���N�^
	CMaterial()
		:mVertexNum(0)
		, mpTextureFilename(nullptr)
	{}
	//�}�e���A����L���ɂ���
	void Enabled();
	//�e�N�X�`����ǂݍ���
	void LoadTexture(char *file);
	//�}�e���A���𖳌��ɂ���
	void Disabled();
	float mPower;
	float mSpecular[3];
	float mEmissive[3];
	//�e�N�X�`���t�@�C����
	char*mpTextureFilename;

	CMaterial(CModelX*model);
	~CMaterial(){
		if (mpTextureFilename){
			delete[]mpTextureFilename;
		}
		mpTextureFilename = nullptr;
	}
};

#endif
