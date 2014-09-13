#pragma once

//
// �e��ݒ�
//

// �⏕�v���O����
#include "gg.h"
using namespace gg;

// 3D �e�N�X�`���̃p�^�[��
#define CHECKER       0
#define SPHERE        1
#define NOISE         2
#define PERLIN        3
#define TURBULENCE    4

// �g�p���� 3D �e�N�X�`���̃p�^�[��
#define VOLUMETYPE    TURBULENCE

// 3D �e�N�X�`���̃T�C�Y
const GLsizei texWidth(32);
const GLsizei texHeight(32);
const GLsizei texDepth(32);

// �X���C�X��
const int slices(256);

// ���̎��̐ݒ�
#define NONE          0                                 // �P�ᎋ
#define LINEBYLINE    1                                 // �C���^�[���[�X�i���T�|�[�g�j
#define TOPANDBOTTOM  2                                 // �㉺
#define SIDEBYSIDE    3                                 // ���E
#define QUADBUFFER    4                                 // �N���b�h�o�b�t�@�X�e���I
#define OCULUS        5                                 // Oculus Rift (HMD)

// ���̎��̕���
#define STEREO        NONE

// ���̎����L�̃p�����[�^
#if STEREO != NONE
const int useSecondary(1);                              // 1 �Ȃ�Z�J���_�����j�^�ɕ\��
const GLfloat initialParallax(0.032f);                  // �����̏����l (�P�� m)
const GLfloat parallaxStep(0.001f);                     // �����̕ύX�X�e�b�v (�P�� m)
const GLfloat screenDistance(2.0f);                     // ���ۂ̃X�N���[���܂ł̋��� (�P�� m)

// Oculus Rift �ŗL�̃p�����[�^
#  if STEREO == OCULUS
const GLfloat lensScaleStep(0.001f);                    // �����Y�̊g�嗦�̕␳�W���̒����X�e�b�v
const GLfloat projectionCenterOffsetStep(0.001f);       // �����Y�̒��S�ʒu�̒����X�e�b�v
const GLuint fboWidth(1024), fboHeight(1024);           // �␳�Ɏg�� FBO �̃T�C�Y
#  endif
#endif

// �J�����̏������
const GLfloat startPosition[] = { 0.0f, 0.0f, -2.0f };  // �{�����[���f�[�^�̒��S�̏����ʒu
const GLfloat displayCenter(0.5f);                      // �f�B�X�v���C�̒��S�ʒu (�����̔���
const GLfloat displayDepth(1.5f);                       // �ϑ��҂ƃf�B�X�v���C�ʂƂ̋���
const GLfloat zNear(0.1f);                              // �O���ʂ܂ł̋���
const GLfloat zFar(10.0f);                              // ����ʂ܂ł̋���

// �i�r�Q�[�V�����̑��x����
const GLfloat speedScale(0.00005f);                     // �t���[��������̈ړ����x�W��
const GLfloat threasholdStep(0.01f);                    // 臒l�̒����W��
const GLfloat axesSpeedScale(0.01f);                    // �Q�[���p�b�h�̃X�e�B�b�N�̑��x�̌W��
const GLfloat axesAngleScale(0.03f);                    // �Q�[���p�b�h�̃X�e�B�b�N�̊p���x�̌W��
const GLfloat btnsScale(0.005f);                        // �Q�[���p�b�h�̃{�^���̌W��

// ���E�F (Oculus Rift �\�����̕\���͈͊O�̐F)
const GLfloat border[] = { 0.0, 0.0, 0.0, 0.0 };

// �w�i�F
const GLfloat back[] = { 0.2f, 0.3f, 0.4f, 0.0f };
