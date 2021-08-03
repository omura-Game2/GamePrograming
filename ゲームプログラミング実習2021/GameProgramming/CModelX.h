#ifndef CMODELX_H
#define CMODELX_H

#define MODEL_FILE "���O�i.x"

//�̈������}�N����
#define SAFE_DELETE_ARRAY(a){if(a)delete[]a;a=0;}

#include<vector>
#include"CMatrix.h"
#include"CVector.h"
//�z��̃T�C�Y�擾���}�N����
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))
class CModelX; //�N���X�̐錾
class CMaterial;//�N���X�̐錾
/*
CSkinWeights
�X�L���E�F�C�g�N���X
*/
class CSkinWeights{
public:
	char*mpFrameName;  //�t���[����
	int mFrameIndex;  //�t���[���ԍ�
	int mIndexNum;  //���_�ԍ�
	int *mpIndex;  //���_�ԍ��z��
	float *mpWeight; //���_�E�F�C�g�z��
	CMatrix mOffset; //�I�t�Z�b�g�}�g���b�N�X

	CSkinWeights(CModelX*model);

	~CSkinWeights(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}
};

//CMesh�N���X�̒�`
class CMesh{
public:
	int mVertexNum;
	CVector *mpVertex;
	int mFaceNum;
	int*mpVertexIndex;
	int mNormalNum;
	CVector*mpNormal;
	int mMaterialNum;//�}�e���A����
	int mMaterialIndexNum;//�}�e���A���ԍ�(�ʐ�)
	int*mpMaterialIndex;//�}�e���A���ԍ�
	std::vector<CMaterial*>mMaterial;//�}�e���A���f�[�^
	std::vector<CSkinWeights*>mSkinWeights;//�X�L���E�F�C�g
	CVector*mpAnimateVertex;  //�A�j���[�V�����p���_
	CVector*mpAnimateNormal;  //�A�j���[�V�����p�@��
	//�e�N�X�`�����W�f�[�^
	float*mpTextureCoords;
	//���_�ɃA�j���[�V�����K�p
	void AnimateVertex(CModelX*model);
	//�R���X�g���N�^
	CMesh()
		:mVertexNum(0)
		, mpVertex(0)
		, mFaceNum(0)
		, mpVertexIndex(nullptr)
		, mNormalNum(0)
		, mpNormal(nullptr)
		, mMaterialNum(0)
		, mMaterialIndexNum(0)
		, mpMaterialIndex(nullptr)
		, mpAnimateVertex(nullptr)
		, mpAnimateNormal(nullptr)
		, mpTextureCoords(nullptr)
	{}
	//�f�X�g���N�^
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		SAFE_DELETE_ARRAY(mpAnimateVertex);
		SAFE_DELETE_ARRAY(mpAnimateNormal);
		SAFE_DELETE_ARRAY(mpTextureCoords);
		//�X�L���E�F�C�g�̍폜
		for (int i = 0; i < mSkinWeights.size(); i++){
			delete mSkinWeights[i];
		}
	}
	void Render();
	//�ǂݍ��ݏ���
	void Init(CModelX*model);
};

/*
CModelXFrame
*/
class CModelXFrame{
public:
	std::vector<CModelXFrame*>mChild;
	CMatrix mTransformMatrix;  //�ϊ��s��
	char*mpName;
	int mIndex;
	CMesh mMesh;
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�����s��
	CMatrix mCombinedMatrix;
	//�����s��̍쐬
	void AnimateCombined(CMatrix*parent);
	//�f�X�g���N�^
	~CModelXFrame(){
		//�q�t���[�������ׂĉ������
		std::vector<CModelXFrame*>::iterator itr;
		for (itr = mChild.begin(); itr != mChild.end(); itr++){
			delete *itr;
		}
		//���O�̃G���A���������
		SAFE_DELETE_ARRAY(mpName);
	}
	void CModelXFrame::Render(){
		if (mMesh.mFaceNum != 0)
			mMesh.Render();
	}
};

/*
CAnimationKey
�A�j���[�V�����L�[�N���X
*/
class CAnimationKey{
public:
	//����
	float mTime;
	//�s��
	CMatrix mMatrix;
};
/*
CAnimation
�A�j���[�V�����N���X
*/
class CAnimation{
public:
	char*mpFrameName;//�t���[����
	int mFrameIndex; //�t���[���ԍ�
	int mKeyNum; //�L�[��(���Ԑ�)
	CAnimationKey *mpKey; //�L�[�̔z��

	CAnimation(CModelX*model);

	~CAnimation(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpKey);
	}
};

/*
CAnimationSet
�A�j���[�V�����Z�b�g
*/
class CAnimationSet{
public:
	//�A�j���[�V�����Z�b�g��
	char*mpName;
	float mTime; //���ݎ���
	float mWeight; //�d��
	float mMaxTime; //�ő厞��

	CAnimationSet(CModelX*model);
	//�A�j���[�V����
	std::vector<CAnimation*>mAnimation;

	~CAnimationSet(){
		SAFE_DELETE_ARRAY(mpName);
		//�A�j���[�V�����v�f�̍폜
		for (int i = 0; i < mAnimation.size(); i++){
			delete mAnimation[i];
		}
	}

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
	//�A�j���[�V�����Z�b�g�̔z��
	std::vector<CAnimationSet*>mAnimationSet;
	std::vector<CMaterial*>mMaterial;

	CModelX()
		:mpPointer(0)
	{}

	~CModelX(){
		if (mFrame.size() > 0)
		{
			delete mFrame[0];
		}
		for (int i = 0; i < mAnimationSet.size(); i++){
			delete mAnimationSet[i];
		}
		//�}�e���A���̉��
		for (int i = 0; i < mMaterial.size(); i++){
			delete mMaterial[i];
		}
	}
	//�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
	CModelXFrame*FindFrame(char*name);
	//�}�e���A���̌���
	CMaterial*FindMaterial(char*name);
	//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
	void SetSkinWeightFrameIndex();
	//���_�ɃA�j���[�V������K�p
	void AnimateVertex();
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
	void Render();

	/*
	AnimateFrame
	�t���[���̕ϊ��s����A�j���[�V�����f�[�^�ōX�V����
	*/
	void CModelX::AnimateFrame(){
		//�A�j���[�V�����œK�p�����t���[����
		//�ϊ��s����[���ɂ���
		for (int i = 0; i < mAnimationSet.size(); i++){
			CAnimationSet*anim = mAnimationSet[i];
			//�d�݂�0�͔�΂�
			if (anim->mWeight == 0)continue;
			//�t���[����(Animation��)�J��Ԃ�
			for (int j = 0; j < anim->mAnimation.size(); j++){
				CAnimation*animation = anim->mAnimation[j];
				//�Y������t���[���̕ϊ��s����[���N���A����
				memset(&mFrame[animation->mFrameIndex]->mTransformMatrix, 0, sizeof(CMatrix));
			}
		}
		//�A�j���[�V�����ɊY������t���[���̕ϊ��s���
		//�A�j���[�V�����f�[�^�Őݒ肷��
		for (int i = 0; i < mAnimationSet.size(); i++){
			CAnimationSet*anim = mAnimationSet[i];
			//�d�݂�0�͔�΂�
			if (anim->mWeight == 0)continue;
			//�t���[����(Animation��)�J��Ԃ�
			for (int j = 0; j < anim->mAnimation.size(); j++){
				//�t���[�����擾����
				CAnimation*animation = anim->mAnimation[j];
				CModelXFrame*frame = mFrame[animation->mFrameIndex];
				//�L�[���Ȃ��ꍇ�͔�΂�
				if (animation->mpKey == 0)continue;
				//���Ԃ��擾
				float time = anim->mTime;
				//�ŏ��̎��Ԃ�菬�����ꍇ
				if (time < animation->mpKey[0].mTime){
					//�ϊ��s���0�R�}�ڂ̍s��ōX�V
					frame->mTransformMatrix += animation->mpKey[0].mMatrix * anim->mWeight;
				}

				//�Ō�̎��Ԃ��傫���ꍇ
				else if (time >= animation->mpKey[animation->mKeyNum - 1].mTime){
					//�ϊ��s����Ō�̃R�}�̍s��ōX�V
					frame->mTransformMatrix += animation->mpKey[animation->mKeyNum - 1].mMatrix * anim->mWeight;
				}
				else{
					//���Ԃ̓r���̏ꍇ
					for (int k = 1; k < animation->mKeyNum; k++){
						//�ϊ��s����A���`��ԂɂčX�V
						if (time < animation->mpKey[k].mTime&&
							animation->mpKey[k - 1].mTime != animation->mpKey[k].mTime){
							float r = (animation->mpKey[k].mTime - time) /
								(animation->mpKey[k].mTime - animation->mpKey[k - 1].mTime);
							frame->mTransformMatrix +=
								(animation->mpKey[k - 1].mMatrix * r + animation->mpKey[k].mMatrix * (1 - r))*anim->mWeight;
							break;
						}
					}
				}
			}
		}
		#ifdef _DEBUG
			for (int i = 0; i < mAnimationSet.size(); i++){
				CAnimationSet*anim = mAnimationSet[i];
				//�d�݂�0�͔�΂�
				if (anim->mWeight == 0)continue;
				//�t���[����(Animation��)�J��Ԃ�
				for (int j = 0; j < anim->mAnimation.size(); j++){
					//�t���[�����擾����
					CAnimation*animation = anim->mAnimation[j];
					CModelXFrame*frame = mFrame[animation->mFrameIndex];
					//printf("Frame:%s\n", frame->mpName);
					frame->mTransformMatrix.Print();
				}
			}
		#endif
	}
};



#endif