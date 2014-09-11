#pragma once

//
// �E�B���h�E�֘A�̏���
//

// �W�����C�u����
#include <cmath>

// �e��ݒ�
#include "config.h"

// Oculus Rift SDK ���C�u���� (LibOVR) �̑g�ݍ���
#if STEREO == OCULUS
#  ifdef _WIN32
#    ifdef _DEBUG
#      pragma comment(lib, "libovrd.lib")
#    else
#      pragma comment(lib, "libovr.lib")
#    endif
#    pragma comment(lib, "winmm.lib")
#  endif
#  ifdef APIENTRY
#    undef APIENTRY
#  endif
#  include <OVRVersion.h>
#  include <OVR.h>
using namespace OVR;
#endif

//
// �E�B���h�E�֘A�̏�����S������N���X
//
class Window
{
  // �E�B���h�E�̎��ʎq
  GLFWwindow *const window;

  // �W���C�X�e�B�b�N�̔ԍ�
  int joy;

  // �X�e�B�b�N�̒����ʒu
  float origin[4];

  // �h���b�O�J�n�ʒu
  double cx, cy;

  // �J�����̈ʒu
  GLfloat ex, ey, ez;

  // ���̂̉�]
  GgTrackball tb;

  // 臒l
  GLfloat threshold;

  // �A���t�@�u�����f�B���O
  bool blend;

  // ���f���r���[�ϊ��s��
  GgMatrix mv;

  // �X�N���[���̕��ƍ���
  GLfloat scrW, scrH;

#if STEREO == NONE
  // ���e�ϊ��s��
  GgMatrix mp;

  //
  // �������e�ϊ��s������߂�
  //
  //   �E�E�B���h�E�̃T�C�Y�ύX����J�����p�����[�^�̕ύX���ɌĂяo��
  //
  void updateProjectionMatrix()
  {
    mp.loadFrustum(-scrW, scrW, -scrH, scrH, zNear, zFar);
  }
#else
  // �E�B���h�E�̕��ƍ���
  int winW, winH;

  // ����
  GLfloat parallax;

  // ���̎��p�̓��e�ϊ��s��
  GgMatrix mpL, mpR;

  //
  // ���̎��p�̓������e�ϊ��s������߂�
  //
  //   �E�E�B���h�E�̃T�C�Y�ύX����J�����p�����[�^�̕ύX���ɌĂяo��
  //
  void updateStereoProjectionMatrix()
  {
    // �����ɂ��X�N���[���̃I�t�Z�b�g��
#  if STEREO == OCULUS
    const GLfloat shift(0.0f);
#  else
    const GLfloat shift(parallax * zNear / screenDistance);
#  endif

    // ���̎��p�̓������e�ϊ��s��
    mpL.loadFrustum(-scrW + shift, scrW + shift, -scrH, scrH, zNear, zFar);
    mpR.loadFrustum(-scrW - shift, scrW - shift, -scrH, scrH, zNear, zFar);
  }
#endif

#if STEREO == OCULUS
  // Oculus Rift �\���p�� FBO
  GLuint ocuFbo[2];

  // Oculus Rift �\���p�� FBO �̃J���[�o�b�t�@�Ɏg���e�N�X�`��
  GLuint ocuFboColor[2];

  // Oculus Rift �\���p�� FBO �̃f�v�X�o�b�t�@�Ɏg�������_�[�o�b�t�@
  GLuint ocuFboDepth;

  // Oculus Rift �̉�ʂ̃A�X�y�N�g��
  GLfloat ocuAspect;

  // Oculus Rift �̃����Y�̒��S�̉�ʂ̒��S����̂���
  GLfloat projectionCenterOffset;

  // Oculus Rift �̃����Y�̘c�݂̕␳�W��
  GLfloat lensDistortion[4];

  // Oculus Rift �̃����Y�̊g�嗦�̕␳�W��
  GLfloat lensScale;

  // Oculus Rift �\���Ɏg����`
  static GLuint ocuVao, ocuVbo;

  // Oculus Rift �\���p�̃V�F�[�_�v���O����
  static GLuint ocuProgram;

  // Oculus Rift �\���p�� FBO �̃e�N�X�`�����j�b�g�� uniform �ϐ��̏ꏊ
  static GLint ocuFboColorLoc;

  // Oculus Rift �̉�ʂ̃A�X�y�N�g��� uniform �ϐ��̏ꏊ
  static GLint ocuAspectLoc;

  // Oculus Rift �̃����Y�̒��S�̉�ʂ̒��S����̂���� uniform �ϐ��̏ꏊ
  static GLint projectionCenterOffsetLoc;

  // Oculus Rift �̃����Y�̘c�݂̕␳�W���� uniform �ϐ��̏ꏊ
  static GLint lensDistortionLoc;

  // Oculus Rift �̃����Y�̊g�嗦�̕␳�W���� uniform �ϐ��̏ꏊ
  static GLint lensScaleLoc;

  // Oculus Rift �\���p�� FBO �̃����_�[�^�[�Q�b�g
  static const GLenum ocuFboDrawBuffers[];

  // Oculus Rift �̃w�b�h�g���b�L���O�Z���T
  static Ptr<DeviceManager> pManager;
  Ptr<HMDDevice> pHmd;
  Ptr<SensorDevice> pSensor;
  HMDInfo hmdInfo;
  SensorFusion sensorFusion;
#endif

  // �Q�ƃJ�E���g
  static unsigned int count;

  //
  // �R�s�[�R���X�g���N�^ (�R�s�[�֎~)
  //
  Window(const Window &w);

  //
  // ��� (�R�s�[�֎~)
  //
  Window &operator=(const Window &w);

public:

  //
  // �R���X�g���N�^
  //
  Window(int width = 640, int height = 480, const char *title = "GLFW Window",
    GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr);

  //
  // �f�X�g���N�^
  //
  virtual ~Window();

  //
  // �E�B���h�E�̎��ʎq�̎擾
  //
  const GLFWwindow *get() const
  {
    return window;
  }

  //
  // �E�B���h�E�����ׂ����𔻒肷��
  //
  //   �E�`�惋�[�v�̌p�������Ƃ��Ďg��
  //
  bool shouldClose() const
  {
    // �E�B���h�E����邩 ESC �L�[���^�C�v����Ă���ΐ^
    return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
  }

  //
  // ��ʃN���A
  //
  //   �E�}�`�̕`��J�n�O�ɌĂяo��
  //   �E��ʂ̏����Ȃǂ��s��
  //
  void clear();

  //
  // �J���[�o�b�t�@�����ւ��ăC�x���g�����o��
  //
  //   �E�}�`�̕`��I����ɌĂяo��
  //   �E�_�u���o�b�t�@�����O�̃o�b�t�@�̓���ւ����s��
  //   �E�L�[�{�[�h���쓙�̃C�x���g�����o��
  //
  void swapBuffers();

  //
  // �E�B���h�E�̃T�C�Y�ύX���̏���
  //
  //   �E�E�B���h�E�̃T�C�Y�ύX���ɃR�[���o�b�N�֐��Ƃ��ČĂяo�����
  //   �E�E�B���h�E�̍쐬���ɂ͖����I�ɌĂяo��
  //
  static void resize(GLFWwindow *window, int width, int height);

  //
  // �}�E�X�{�^���𑀍삵���Ƃ��̏���
  //
  //   �E�}�E�X�{�^�����������Ƃ��ɃR�[���o�b�N�֐��Ƃ��ČĂяo�����
  //
  static void mouse(GLFWwindow *window, int button, int action, int mods);

  //
  // �}�E�X�z�C�[�����쎞�̏���
  //
  //   �E�}�E�X�z�C�[���𑀍삵�����ɃR�[���o�b�N�֐��Ƃ��ČĂяo�����
  //
  static void wheel(GLFWwindow *window, double x, double y);

  //
  // �L�[�{�[�h���^�C�v�������̏���
  //
  //   �D�L�[�{�[�h���^�C�v�������ɃR�[���o�b�N�֐��Ƃ��ČĂяo�����
  //
  static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);

#if STEREO == NONE
  //
  // ���f���r���[�ϊ��s��𓾂�
  //
  const GgMatrix &getMv() const
  {
    return mv;
  }

  //
  // �v���W�F�N�V�����ϊ��s��𓾂�
  //
  const GgMatrix &getMp() const
  {
    return mp;
  }

  //
  // ���f���r���[�v���W�F�N�V�����ϊ��s��𓾂�
  //
  GgMatrix getMc() const
  {
    return mp * mv;
  }
#else
  //
  // ���ڗp�̃��f���r���[�ϊ��s��𓾂�
  //
  //   �E���ڂ̕`����L�̏������s��
  //
  GgMatrix getMvL() const;

  //
  // ���ڗp�̃v���W�F�N�V�����ϊ��s��𓾂�
  //
  const GgMatrix &getMpL() const
  {
    return mpL;
  }

  //
  // ���ڗp�̃��f���r���[�v���W�F�N�V�����ϊ��s��𓾂�
  //
  GgMatrix getMcL() const
  {
    return mpL * getMvL();
  }

  //
  // �E�ڗp�̃��f���r���[�ϊ��s��𓾂�
  //
  //   �E�E�ڂ̕`����L�̏������s��
  //
  GgMatrix getMvR() const;

  //
  // �E�ڗp�̃v���W�F�N�V�����ϊ��s��𓾂�
  //
  const GgMatrix &getMpR() const
  {
    return mpR;
  }

  //
  // �E�ڗp�̃��f���r���[�v���W�F�N�V�����ϊ��s��𓾂�
  //
  GgMatrix getMcR() const
  {
    return mpR * getMvR();
  }
#endif

  //
  // ���̂̉�]�𓾂�
  //
  const GgTrackball &getTb() const
  {
    return tb;
  }

  //
  // �A���t�@�l�𓾂�
  //
  GLfloat getThreshold() const
  {
    return threshold;
  }
};
