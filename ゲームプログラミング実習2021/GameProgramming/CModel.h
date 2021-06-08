#ifndef CMODEL_H
#define CMODEL_H
//vector�̃C���N���[�h
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include"CModelX.h"

/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel {
public:
	//���_�z��
	float *mpVertex; //���_���W
	float *mpNormal; //�@��
	float *mpTextureCoord; //�e�N�X�`���}�b�s���O
	//�f�t�H���g�R���X�g���N�^
	CModel();
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle> mTriangles;
	//�}�e���A���|�C���^�̉ϒ��z��
	std::vector<CMaterial*> mpMaterials;
	//�e�N�X�`���}�b�s���O�̕ۑ�(CVector�^)
	std::vector<CVector> uv;

	~CModel();

	//���f���t�@�C���̓���
	//Load(���f���t�@�C����, �}�e���A���t�@�C����)
	void Load(char *obj, char *mtl);
	//�`��
	void Render();
	//�`��
	//Render(�s��)
	void Render(const CMatrix &m);
};
/*
CModelX
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX{
public:
	char*mpPointer;
	char mToken[1024];
	std::vector<CModelXFrame*>mFrame;

	CModelX()
		:mpPointer(0)
	{}

	~CModelX(){
		if (mFrame.size() > 0)
		{
			delete mFrame[0];
		}
	}

	//�t�@�C���ǂݍ���
	void Load(char*file);
	//�P��̎��o��
	void GetToken();
	//�m�[�h�̓ǂݍ���
	void SkipNode();
	//���������_�f�[�^�̎擾
	float GetFloatToken();
	//�����f�[�^�̎擾
	int GetIntToken();
	/*
	Render
	�S�Ẵt���[���̕`�揈�����Ăяo��
	*/
	void CModelX::Render(){
		for (int i = 0; i < mFrame.size(); i++){
			mFrame[i]->Render();
		}
	}
};
#endif
