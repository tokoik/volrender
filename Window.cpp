//
// �E�B���h�E�֘A�̏���
//
#include <iostream>
#include "Window.h"

// �W���C�X�e�B�b�N�� Mac �ɂȂ��ƉE���̃X�e�B�b�N�̔ԍ���������
#ifdef __APPLE__
const int appleOffset(1);
#else
const int appleOffset(0);
#endif

//
// �R���X�g���N�^
//
Window::Window(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
  : window(glfwCreateWindow(width, height, title, monitor, share))
  , ex(startPosition[0])                  // �J������ x ���W
  , ey(startPosition[1])                  // �J������ y ���W
  , ez(startPosition[2])                  // �J������ z ���W
  , threshold(0.5f)                       // 臒l
  , blend(true)                           // �A���t�@�u�����f�B���O
#if STEREO != OCULUS && STEREO != NONE
  , parallax(initialParallax)
#endif
#if STEREO != OCULUS
  , scrH(zNear * displayCenter / displayDepth)
#endif
{
  if (!window) return;

  // ���݂̃E�B���h�E�������Ώۂɂ���
  glfwMakeContextCurrent(window);

  // �쐬�����E�B���h�E�ɑ΂���ݒ�
  glfwSwapInterval(1);

  // �E�B���h�E�̃T�C�Y�ύX���ɌĂяo�������̓o�^
  glfwSetFramebufferSizeCallback(window, resize);

  // �}�E�X�{�^���𑀍삵���Ƃ��̏���
  glfwSetMouseButtonCallback(window, mouse);

  // �}�E�X�z�C�[�����쎞�ɌĂяo������
  glfwSetScrollCallback(window, wheel);

  // �L�[�{�[�h�𑀍삵�����̏���
  glfwSetKeyCallback(window, keyboard);

  // �}�E�X�J�[�\����\������
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // ���̃C���X�^���X�� this �|�C���^���L�^���Ă���
  glfwSetWindowUserPointer(window, this);

  // �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
  if (!glCreateProgram) ggInit();

  // �W���C�X�e�b�N�̗L���𒲂ׂĔԍ������߂�
  joy = glfwJoystickPresent(count) ? count : -1;

  // �X�e�B�b�N�̒����ʒu�����߂�
  if (joy >= 0)
  {
    int axesCount;
    const float *const axes(glfwGetJoystickAxes(joy, &axesCount));

    if (axesCount > 3 + appleOffset)
    {
      // �N������̃X�e�B�b�N�̈ʒu����ɂ���
      origin[0] = axes[0];
      origin[1] = axes[1];
      origin[2] = axes[2 + appleOffset];
      origin[3] = axes[3 + appleOffset];
    }
  }

#if STEREO == OCULUS
  // �v���O�����I�u�W�F�N�g, VAO / VBO, Oculus Rift �̃f�o�C�X�}�l�[�W���[�̍쐬�͍ŏ���x�����s��
  if (!count)
  {
    // Oculus Rift �̃����Y�̘c�݂�␳����V�F�[�_�v���O����
    ocuProgram = ggLoadShader("oculus.vert", "oculus.frag");
    ocuFboColorLoc = glGetUniformLocation(ocuProgram, "ocuFboColor");
    ocuAspectLoc = glGetUniformLocation(ocuProgram, "ocuAspect");
    projectionCenterOffsetLoc = glGetUniformLocation(ocuProgram, "projectionCenterOffset");
    lensDistortionLoc = glGetUniformLocation(ocuProgram, "lensDistortion");
    lensScaleLoc = glGetUniformLocation(ocuProgram, "lensScale");

    // Oculus Rift �\���Ɏg����`
    glGenVertexArrays(1, &ocuVao);
    glBindVertexArray(ocuVao);
    glGenBuffers(1, &ocuVbo);
    glBindBuffer(GL_ARRAY_BUFFER, ocuVbo);
    static const GLfloat rect[] = { -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };
    glBufferData(GL_ARRAY_BUFFER, sizeof rect, rect, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Oculus Rift �̃f�o�C�X�}�l�[�W���[�̍쐬
    pManager = *DeviceManager::Create();
  }

  // Oculus Rift �̃f�o�C�X�}�l�[�W���[���쐬�ł���������擾����
  if (pManager
    && (pHmd = *pManager->EnumerateDevices<HMDDevice>().CreateDevice())
    && pHmd->GetDeviceInfo(&hmdInfo)
    )
  {
#  ifdef _DEBUG
    // �擾��������\������
    std::cout << hmdInfo.DisplayDeviceName << std::endl;
    std::cout << "\nResolution:"
      << hmdInfo.HResolution << ", "
      << hmdInfo.VResolution << std::endl;
    std::cout << "\nScreen size: "
      << hmdInfo.HScreenSize << ", "
      << hmdInfo.VScreenSize << std::endl;
    std::cout << "\nVertical Screen Center: "
      << hmdInfo.VScreenCenter << std::endl;
    std::cout << "\nEye to Screen Distance: "
      << hmdInfo.EyeToScreenDistance << std::endl;
    std::cout << "\nLens Separation Distance: "
      << hmdInfo.LensSeparationDistance << std::endl;
    std::cout << "\nInterpupillary Distance: "
      << hmdInfo.InterpupillaryDistance << std::endl;
    std::cout << "\nDistortion: "
      << hmdInfo.DistortionK[0] << ", "
      << hmdInfo.DistortionK[1] << ", "
      << hmdInfo.DistortionK[2] << ", "
      << hmdInfo.DistortionK[3] << std::endl;
    std::cout << std::endl;
#  endif

    // �����Y�̒��S�̉�ʂ̒��S����̂���
    projectionCenterOffset = 1.0f - 2.0f * hmdInfo.LensSeparationDistance / hmdInfo.HScreenSize;

    // �X�N���[���̕��ƍ���
    scrW = scrH = zNear * hmdInfo.VScreenCenter / hmdInfo.EyeToScreenDistance;

    // ����
    parallax = hmdInfo.InterpupillaryDistance * 0.5f;

    // �����Y�̘c�݂̕␳�W��
    lensDistortion[0] = hmdInfo.DistortionK[0];
    lensDistortion[1] = hmdInfo.DistortionK[1];
    lensDistortion[2] = hmdInfo.DistortionK[2];
    lensDistortion[3] = hmdInfo.DistortionK[3];

    // �Жڂ̕\���̈�̃A�X�y�N�g��
    ocuAspect = hmdInfo.HScreenSize * 0.5f / hmdInfo.VScreenSize;

    // Oculus Rift �̃Z���T�̎擾
    pSensor = *pHmd->GetSensor();

    // �Z���T�[��o�^����
    if (pSensor) sensorFusion.AttachToSensor(pSensor);
  }
  else
  {
    // Oculus Rift ���Ȃ����Ƀf�o�b�O���鎞�̐ݒ�
    scrW = scrH = zNear * 0.0468f / 0.041f;
    parallax = 0.064f * 0.5f;
    projectionCenterOffset = 1.0f - 2.0f * 0.0635f / 0.14976f;
    lensDistortion[0] = 1.0f;
    lensDistortion[1] = 0.2f;
    lensDistortion[2] = 0.24f;
    lensDistortion[3] = 0.0f;
    ocuAspect = 0.14976f * 0.5f / 0.0936f;
    pSensor = nullptr;
  }

  // �����Y�̘c�ݕ␳�ɔ����g�嗦�̕␳
  lensScale = 1.0f / (lensDistortion[0] + lensDistortion[1] + lensDistortion[2] + lensDistortion[3]);

  // Oculus Rift �̍��ڗp�ƉE�ڗp�� FBO �̏���
  glGenFramebuffers(2, ocuFbo);

  // Oculus Rift �\���p�� FBO �̃f�v�X�o�b�t�@�Ƃ��Ďg�������_�[�o�b�t�@�̍쐬
  glGenRenderbuffers(1, &ocuFboDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, ocuFboDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fboWidth, fboHeight);

  // Oculus Rift �\���p�� FBO �̃J���[�o�b�t�@�Ƃ��Ďg���J���[�e�N�X�`���̍쐬
  glGenTextures(2, ocuFboColor);
  for (int i = 0; i < 2; ++i)
  {
    // ���E�̖ڂ̂��ꂼ��̕\���T�C�Y��菭���傫�ȃe�N�X�`���������̊m��
    glBindTexture(GL_TEXTURE_2D, ocuFboColor[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fboWidth, fboHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    // ���E�̖ڂ̂��ꂼ��ɂ��� FBO ���쐬����
    glBindFramebuffer(GL_FRAMEBUFFER, ocuFbo[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, ocuFboColor[i], 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_RENDERBUFFER, ocuFboDepth);
  }
#endif

  // �Q�ƃJ�E���g�𑝂₷
  ++count;

  // ���e�ϊ��s��E�r���[�|�[�g������������
  resize(window, width, height);
}

//
// �f�X�g���N�^
//
Window::~Window()
{
#if STEREO == OCULUS
  // �v���O�����I�u�W�F�N�g, VAO / VBO, Oculus Rift �̃f�o�C�X�}�l�[�W���[�͍Ō�ɍ폜����
  if (--count)
  {
    // �v���O�����I�u�W�F�N�g�̍폜
    glDeleteProgram(ocuProgram);

    // VAO �̍폜
    glDeleteBuffers(1, &ocuVbo);
    glDeleteVertexArrays(1, &ocuVao);

    // Oculus Rift �̃f�o�C�X�}�l�[�W���[�̍폜
    pManager->Release();
    System::Destroy();
  }

  // FBO �̍폜
  glDeleteTextures(1, &ocuFboDepth);
  glDeleteTextures(2, ocuFboColor);
  glDeleteFramebuffers(2, ocuFbo);
#endif

  glfwDestroyWindow(window);
}

//
// ��ʃN���A
//
//   �E�}�`�̕`��J�n�O�ɌĂяo��
//   �E��ʂ̏����Ȃǂ��s��
//
void Window::clear()
{
#if STEREO == OCULUS
  // �B�ʏ���������L���ɂ���
  glEnable(GL_DEPTH_TEST);

  // FBO �S�̂��r���[�|�[�g�ɂ���
  glViewport(0, 0, fboWidth, fboHeight);

  // �Z���T�[�L�����̏���
  if (pSensor)
  {
    // Oculus Rift �̌������擾����
    const Quatf o(sensorFusion.GetOrientation());

    // Oculus Rift �̌����̉�]�̕ϊ��s������߂�
    const GgMatrix mo(ggQuaternionTransposeMatrix(GgQuaternion(o.x, o.y, o.z, o.w)));

    // Oculus Rift �̌��������f���r���[�ϊ��s��ɔ��f����
    mv = mo.translate(-ex, -ey, -ez);
  }
  else
  {
    // ���f���r���[�ϊ��s���ݒ肷��
    mv = ggTranslate(-ex, -ey, -ez);
  }
#else
  // ���f���r���[�ϊ��s���ݒ肷��
  mv = ggTranslate(-ex, -ey, -ez);

  // �J���[�o�b�t�@�ƃf�v�X�o�b�t�@������
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

  // �A���t�@�u�����f�B���O�̐ݒ�
  if (blend)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);
}

//
// �J���[�o�b�t�@�����ւ��ăC�x���g�����o��
//
//   �E�}�`�̕`��I����ɌĂяo��
//   �E�_�u���o�b�t�@�����O�̃o�b�t�@�̓���ւ����s��
//   �E�L�[�{�[�h���쓙�̃C�x���g�����o��
//
void Window::swapBuffers()
{
#if STEREO == OCULUS
  // �f�B�X�v���C�ɕ`��
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK);

  // �B�ʏ��������͍s��Ȃ�
  glDisable(GL_DEPTH_TEST);

  // �A���t�@�u�����f�B���O���s��Ȃ�
  glDisable(GL_BLEND);

  // �\���̈�𕢂���`
  glBindVertexArray(ocuVao);

  // Oculus Rift �̃����Y�␳�p�V�F�[�_
  glUseProgram(ocuProgram);

  // FBO �ɕ`�悵�����ʂ��Q�Ƃ���e�N�X�`�����j�b�g
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(ocuFboColorLoc, 0);

  // Oculus Rift �̃A�X�y�N�g��
  glUniform1f(ocuAspectLoc, ocuAspect);

  // �����Y�̘c�݂̕␳�W��
  glUniform4fv(lensDistortionLoc, 1, lensDistortion);

  // �����Y�̊g�嗦�̕␳�W��
  glUniform1f(lensScaleLoc, lensScale);

  // ���ڂ̕`��
  glUniform1f(projectionCenterOffsetLoc, -projectionCenterOffset);
  glViewport(0, 0, winW, winH);
  glBindTexture(GL_TEXTURE_2D, ocuFboColor[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // �E�ڂ̕`��
  glUniform1f(projectionCenterOffsetLoc, projectionCenterOffset);
  glViewport(winW, 0, winW, winH);
  glBindTexture(GL_TEXTURE_2D, ocuFboColor[1]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
#endif

  // �G���[�`�F�b�N
  ggError("SwapBuffers");

  // �J���[�o�b�t�@�����ւ���
  glfwSwapBuffers(window);

  // �C�x���g�����o��
  glfwPollEvents();

  // �}�E�X�̈ʒu�𒲂ׂ�
  double x, y;
  glfwGetCursorPos(window, &x, &y);

  // ���{�^���h���b�O
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
  {
    // �J�����̈ʒu���ړ�����
    ex += speedScale * GLfloat(x - cx);
    ez += speedScale * GLfloat(y - cy);
  }

  // �E�{�^���h���b�O
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
  {
    // �g���b�N�{�[����]
    tb.motion(GLfloat(x), GLfloat(y));
  }

  // �Q�[���p�b�h�ɂ�鑀��
  if (joy >= 0)
  {
    // �X�e�B�b�N
    int axesCount;
    const float *const axes(glfwGetJoystickAxes(joy, &axesCount));

    if (axesCount > 3 + appleOffset)
    {
      // �J�����̈ʒu���ړ�����
      ex += (axes[0] - origin[0]) * axesSpeedScale;
      ez += (axes[1] - origin[1]) * axesSpeedScale;

      // ���̂���]����
      tb.rotate(ggRotateQuaternion(0.0f, 1.0f, 0.0f, (axes[2 + appleOffset] - origin[2]) * axesAngleScale));
      tb.rotate(ggRotateQuaternion(1.0f, 0.0f, 0.0f, (axes[3 + appleOffset] - origin[3]) * axesAngleScale));
    }

    // �{�^��
    int btnsCount;
    const unsigned char *const btns(glfwGetJoystickButtons(joy, &btnsCount));
    if (btnsCount > 3)
    {
      // 臒l�𒲐�����
      float t(threshold + GLfloat(btns[2] - btns[1]) * btnsScale);
      if (t >= 0.0f && t <= 1.0f) threshold = t;
    }

    // ���̂̉�]�����ɖ߂�
    if (btnsCount > 4 && btns[4] > 0) tb.reset();
  }

#if STEREO != NONE
  // �����L�[����
  if (glfwGetKey(window, GLFW_KEY_LEFT))
  {
#  if STEREO == OCULUS
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))
    {
      // �����Y�Ԋu���k������
      projectionCenterOffset -= projectionCenterOffsetStep;
    }
    else
    {
      // �������k������
      parallax -= parallaxStep;
      updateStereoProjectionMatrix();
    }
# else
    // �������k������
    parallax -= parallaxStep;
    updateStereoProjectionMatrix();
#  endif
  }

  // �E���L�[����
  if (glfwGetKey(window, GLFW_KEY_RIGHT))
  {
#  if STEREO == OCULUS
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT))
    {
      // �����Y�Ԋu���g�傷��
      projectionCenterOffset += projectionCenterOffsetStep;
    }
    else
    {
      // �������g�傷��
      parallax += parallaxStep;
      updateStereoProjectionMatrix();
    }
# else
    // �������g�傷��
    parallax += parallaxStep;
    updateStereoProjectionMatrix();
#  endif
  }

#  if STEREO == OCULUS
  // �����L�[����
  if (glfwGetKey(window, GLFW_KEY_DOWN))
  {
    // �����Y�̊g�嗦�̕␳�W����������
    lensScale -= lensScaleStep;
  }

  // �E���L�[����
  if (glfwGetKey(window, GLFW_KEY_UP))
  {
    // �����Y�̊g�嗦�̕␳�W�����グ��
    lensScale += lensScaleStep;
  }
#  endif
#endif
}

//
// �E�B���h�E�̃T�C�Y�ύX���̏���
//
//   �E�E�B���h�E�̃T�C�Y�ύX���ɃR�[���o�b�N�֐��Ƃ��ČĂяo�����
//   �E�E�B���h�E�̍쐬���ɂ͖����I�ɌĂяo��
//
void Window::resize(GLFWwindow *window, int width, int height)
{
  // ���̃C���X�^���X�� this �|�C���^�𓾂�
  Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

  if (instance)
  {
    // �g���b�N�{�[�������͈̔͂�ݒ肷��
    instance->tb.region(width, height);

#if STEREO != OCULUS
    // �f�B�X�v���C�̃A�X�y�N�g�� w / h ����X�N���[���̕������߂�
    instance->scrW = instance->scrH * GLfloat(width) / GLfloat(height);
#endif

#if STEREO == SIDEBYSIDE || STEREO == OCULUS
    // �E�B���h�E�̉��������r���[�|�[�g�ɂ���
    width /= 2;
#elif STEREO == TOPANDBOTTOM
    // �E�B���h�E�̏c�������r���[�|�[�g�ɂ���
    height /= 2;
#else
    // �E�B���h�E�S�̂��r���[�|�[�g�ɂ���
    glViewport(0, 0, width, height);
#endif

#if STEREO == NONE
    // ���e�ϊ��s������߂�
    instance->updateProjectionMatrix();
#else
    // �X�e���I�\���̎��̓r���[�|�[�g�̑傫����ۑ����Ă���
    instance->winW = width;
    instance->winH = height;

    // ���e�ϊ��s������߂�
    instance->updateStereoProjectionMatrix();
#endif
  }
}

//
// �}�E�X�{�^���𑀍삵���Ƃ��̏���
//
//   �E�}�E�X�{�^�����������Ƃ��ɃR�[���o�b�N�֐��Ƃ��ČĂяo�����
//
void Window::mouse(GLFWwindow *window, int button, int action, int mods)
{
  // ���̃C���X�^���X�� this �|�C���^�𓾂�
  Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

  if (instance)
  {
    // �}�E�X�̌��݈ʒu�����o��
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    switch (button)
    {
    case GLFW_MOUSE_BUTTON_1:
      if (action)
      {
        // �h���b�O�J�n�ʒu��ۑ�����
        instance->cx = x;
        instance->cy = y;
      }
      break;
    case GLFW_MOUSE_BUTTON_2:
      if (action)
      {
        // �g���b�N�{�[�������J�n
        instance->tb.start(GLfloat(x), GLfloat(y));
      }
      else
      {
        // �g���b�N�{�[�������I��
        instance->tb.stop(GLfloat(x), GLfloat(y));
      }
      break;
    case GLFW_MOUSE_BUTTON_3:
      break;
      break;
    default:
      break;
    }
  }
}

//
// �}�E�X�z�C�[�����쎞�̏���
//
//   �E�}�E�X�z�C�[���𑀍삵�����ɃR�[���o�b�N�֐��Ƃ��ČĂяo�����
//
void Window::wheel(GLFWwindow *window, double x, double y)
{
  // ���̃C���X�^���X�� this �|�C���^�𓾂�
  Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

  if (instance)
  {
    GLfloat t = instance->threshold - threasholdStep * GLfloat(y);
    if (t >= 0.0f && t <= 1.0f) instance->threshold = t;
  }
}

//
// �L�[�{�[�h���^�C�v�������̏���
//
//   �D�L�[�{�[�h���^�C�v�������ɃR�[���o�b�N�֐��Ƃ��ČĂяo�����
//
void Window::keyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  // ���̃C���X�^���X�� this �|�C���^�𓾂�
  Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

  if (instance)
  {
    if (action == GLFW_PRESS)
    {
      switch (key)
      {
      case GLFW_KEY_B:
        instance->blend = !instance->blend;
        break;
      case GLFW_KEY_R:
        // �J���������Z�b�g����
        instance->ex = startPosition[0];
        instance->ey = startPosition[1];
        instance->ez = startPosition[2];
        instance->tb.reset();
        break;
      case GLFW_KEY_SPACE:
        break;
      case GLFW_KEY_BACKSPACE:
      case GLFW_KEY_DELETE:
        break;
      case GLFW_KEY_UP:
        break;
      case GLFW_KEY_DOWN:
        break;
      case GLFW_KEY_RIGHT:
        break;
      case GLFW_KEY_LEFT:
        break;
      default:
        break;
      }
    }
  }
}

#if STEREO != NONE
//
// ���ڗp�̃��f���r���[�ϊ��s��𓾂�
//
//   �E���ڂ̕`����L�̏������s��
//
GgMatrix Window::getMvL() const
{
#  if STEREO == LINEBYLINE
  // �����Ԗڂ̑����������ɕ`�悷��
#  elif STEREO == TOPANDBOTTOM
  // �f�B�X�v���C�̏㔼�������ɕ`�悷��
  glViewport(0, winH, winW, winH);
#  elif STEREO == SIDEBYSIDE
  // �f�B�X�v���C�̍����������ɕ`�悷��
  glViewport(0, 0, winW, winH);
#  elif STEREO == QUADBUFFER
  // ���ڗp�o�b�t�@�ɕ`�悷��
  glDrawBuffer(GL_BACK_LEFT);
#  elif STEREO == OCULUS
  // ���ڗp�� FBO �ɕ`�悷��
  glBindFramebuffer(GL_FRAMEBUFFER, ocuFbo[0]);
  glDrawBuffers(1, ocuFboDrawBuffers);

  // ���ڗp�� FBO ����������
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#  endif

  // ���ڂ����ɓ���������ɃV�[�����E�ɓ�����
  return mv.translate(parallax, 0.0f, 0.0f);
}

//
// �E�ڗp�̃��f���r���[�ϊ��s��𓾂�
//
//   �E�E�ڂ̕`����L�̏������s��
//
GgMatrix Window::getMvR() const
{
#  if STEREO == LINEBYLINE
  // ��Ԗڂ̑����������ɕ`�悷��
#  elif STEREO == TOPANDBOTTOM
  // �f�B�X�v���C�̉����������ɕ`�悷��
  glViewport(0, 0, winW, winH);
#  elif STEREO == SIDEBYSIDE
  // �f�B�X�v���C�̉E���������ɕ`�悷��
  glViewport(winW, 0, winW, winH);
#  elif STEREO == QUADBUFFER
  // �E�ڗp�o�b�t�@�ɕ`�悷��
  glDrawBuffer(GL_BACK_RIGHT);
#  elif STEREO == OCULUS
  // �E�ڗp�� FBO �ɕ`�悷��
  glBindFramebuffer(GL_FRAMEBUFFER, ocuFbo[1]);
  glDrawBuffers(1, ocuFboDrawBuffers);

  // �E�ڗp�� FBO ����������
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#  endif

  // �E�ڂ����ɓ���������ɃV�[�������ɓ�����
  return mv.translate(-parallax, 0.0f, 0.0f);
}

#  if STEREO == OCULUS
// Oculus Rift �\���Ɏg����`
GLuint Window::ocuVao, Window::ocuVbo;

// Oculus Rift �\���p�̃V�F�[�_�v���O����
GLuint Window::ocuProgram;

// Oculus Rift �\���p�� FBO �̃e�N�X�`�����j�b�g�� uniform �ϐ��̏ꏊ
GLint Window::ocuFboColorLoc;

// Oculus Rift �̉�ʂ̃A�X�y�N�g��� uniform �ϐ��̏ꏊ
GLint Window::ocuAspectLoc;

// Oculus Rift �̃����Y�̒��S�̉�ʂ̒��S����̂���� uniform �ϐ��̏ꏊ
GLint Window::projectionCenterOffsetLoc;

// Oculus Rift �̃����Y�̘c�݂̕␳�W���� uniform �ϐ��̏ꏊ
GLint Window::lensDistortionLoc;

// Oculus Rift �̃����Y�̊g�嗦�̕␳�W���� uniform �ϐ��̏ꏊ
GLint Window::lensScaleLoc;

// Oculus Rift �\���p�� FBO �̃����_�[�^�[�Q�b�g
const GLenum Window::ocuFboDrawBuffers[] = { GL_COLOR_ATTACHMENT0 };

// Oculus Rift �̃w�b�h�g���b�L���O�Z���T
Ptr<DeviceManager> Window::pManager;
#  endif
#endif

// �Q�ƃJ�E���g
unsigned int Window::count(0);
