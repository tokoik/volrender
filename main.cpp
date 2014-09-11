// �W�����C�u����
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

// �m�C�Y
#include "Noise3.h"

// �E�B���h�E�֘A�̏���
#include "Window.h"

namespace
{
  //
  // �X���C�X�̍쐬
  //
  GLuint makeSlice()
  {
    // ���_�z��I�u�W�F�N�g���쐬���Č�������
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // ���_�o�b�t�@�I�u�W�F�N�g���쐬���Č�������
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // �o�b�t�@�I�u�W�F�N�g���m�ۂ���
    static const GLfloat p[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
    glBufferData(GL_ARRAY_BUFFER, sizeof p, p, GL_STATIC_DRAW);

    // ���_�ʒu�� index == 0 �� in �ϐ����瓾��
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    return vao;
  }

  //
  // �{�����[���f�[�^�̍쐬
  //
  GLuint makeVolume(GLint width, GLint height, GLint depth, Pattern pattern)
  {
    // ��Ɨp������
    std::vector<GLubyte> texture;

    // �m�C�Y�֐�������������
    const Noise3 noise(5, 5, 5);

    // �m�C�Y�֐����g���ăe�N�X�`�������
    for (GLint k = 0; k < depth; ++k)
    {
      const double z(double(k) / double(depth - 1));

      for (GLint j = 0; j < height; ++j)
      {
        const double y(double(j) / double(height - 1));

        for (GLint i = 0; i < width; ++i)
        {
          const double x(double(i) / double(width - 1));
          GLubyte t;

          switch (pattern)
          {
            case CHECKER:
              t = ((i >> 4) + (j >> 4) + (k >> 4)) & 1 ? 0 : 255;
              break;
            case NOISE:
              t = GLubyte(noise.noise(x, y, z) * 255.0);
              break;
            case PERLIN:
              t = GLubyte(noise.perlin(x, y, z, 4, 0.5) * 255.0);
              break;
            case TURBULENCE:
              t = GLubyte(noise.turbulence(x, y, z, 4, 0.5) * 255.0);
              break;
            case SPHERE:
            {
              const double px(2.0 * x - 1.0), py(2.0 * y - 1.0), pz(2.0 * z - 1.0);
              t = GLubyte(255.0 - sqrt(px * px + py * py + pz * pz) * 127.5);
            }
              break;
            default:
              break;
          }

          texture.push_back(t);
          texture.push_back(t);
          texture.push_back(t);
          texture.push_back(t);
        }
      }
    }

    // �e�N�X�`���I�u�W�F�N�g���쐬���Č�������
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_3D, tex);

    // �e�N�X�`�������蓖�Ă�
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, width, height, depth, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, &texture[0]);

    // �e�N�X�`���̊g��E�k���ɐ��`��Ԃ�p����
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // �e�N�X�`������͂ݏo�������ɂ͋��E�F��p����
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    // �e�N�X�`���̋��E�F��ݒ肷�� (�{�����[���̊O�ɂ͉����Ȃ�)
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, black);

    return tex;
  }

  //
  // ���z�̌v�Z
  //
  GLuint gradient(GLuint vao, GLuint volume, GLint width, GLint height, GLint depth)
  {
    // �e�N�X�`���I�u�W�F�N�g���쐬���Č�������
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_3D, tex);

    // �e�N�X�`�������蓖�Ă�
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, width, height, depth, 0,
      GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // �e�N�X�`���̊g��E�k���ɐ��`��Ԃ�p����
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // �e�N�X�`������͂ݏo�������ɂ͋��E�F��p����
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    // �e�N�X�`���̋��E�F��ݒ肷��
    static const GLfloat white[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, white);

    // ���݂̃r���[�|�[�g��ۑ�����
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    // �r���[�|�[�g���t���[���o�b�t�@�I�u�W�F�N�g�̃T�C�Y�ɍ��킹��
    glViewport(0, 0, width, height);

    // �t���[���o�b�t�@�I�u�W�F�N�g���쐬���Ă����ɕ`�悷��
    GLuint fb;
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    // �`��� GL_COLOR_ATTACHIMENT0 �ɍs��
    static const GLenum bufs[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, bufs);

    // �f�v�X�o�b�t�@�ɂ͕`�悵�Ȃ�
    glDepthMask(GL_FALSE);

    // �A���t�@�u�����f�B���O�͍s��Ȃ�
    glDisable(GL_BLEND);

    // �V�F�[�_�v���O�������쐬����
    const GLuint program(ggLoadShader("gradient.vert", "gradient.frag"));
    const GLint texLoc(glGetUniformLocation(program, "tex"));
    const GLint zLoc(glGetUniformLocation(program, "z"));

    // �V�F�[�_�̎g�p���J�n����
    glUseProgram(program);
    glUniform1i(texLoc, 0);

    // 3D �e�N�X�`�����}�b�s���O����
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, volume);

    // ���C�����Ƃɕ`�悷��
    glBindVertexArray(vao);
    for (GLint z = 0; z < depth; ++z)
    {
      // �t���[���o�b�t�@�I�u�W�F�N�g�ɃJ���[�o�b�t�@�Ƃ��ăA���C�e�N�X�`���̃��C������������
      glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0, z);

      // 3D �e�N�X�`�����N���b�s���O��Ԃ𕢂���`�|���S���Ƀ}�b�s���O���ă��C���ɕ`�悷��
      glUniform1f(zLoc, (GLfloat(z) + 0.5f) / GLfloat(depth));
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // �t���[���o�b�t�@�I�u�W�F�N�g�ւ̕`����I�����Ēʏ�̕`��ɖ߂�
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // �o�b�N�o�b�t�@�ɕ`�悷��
    glDrawBuffer(GL_BACK);

    // �f�v�X�o�b�t�@�ւ̏������݂�������
    glDepthMask(GL_TRUE);

    // �r���[�|�[�g�𕜋A����
    glViewport(vp[0], vp[1], vp[2], vp[3]);

    return tex;
  }

  //
  // �I������
  //
  void cleanup()
  {
    // GLFW �̏I������
    glfwTerminate();
  }
}

//
// ���C���v���O����
//
int main(int argc, const char * argv[])
{
  // GLFW ������������
  if (glfwInit() == GL_FALSE)
  {
    // GLFW �̏������Ɏ��s����
    std::cerr << "Can't initialize GLFW" << std::endl;
    return EXIT_FAILURE;
  }

  // �v���O�����I�����̏�����o�^����
  atexit(cleanup);

#if STEREO == OCULUS
  // Oculus Rift (LibOVR) ������������
  System::Init(Log::ConfigureDefaultLog(LogMask_All));
#endif

  // OpenGL Version 3.2 Core Profile ��I������
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_STEREO, STEREO == QUADBUFFER);

#if STEREO != NONE && !defined(_DEBUG)
  // �ڑ�����Ă��郂�j�^��{��
  int mcount;
  GLFWmonitor **const monitors = glfwGetMonitors(&mcount);

  // �Z�J���_�����j�^������΂�����g��
  GLFWmonitor *const monitor(monitors[mcount > 1 ? useSecondary : 0]);

  // ���j�^�̃��[�h�𒲂ׂ�
  const GLFWvidmode* mode(glfwGetVideoMode(monitor));

  // �t���X�N���[���ŃE�B���h�E���J��
  Window window(mode->width, mode->height, "Volume Rendering (STEREO)", monitor);
#else
  // �E�B���h�E���[�h�ŃE�B���h�E���J��
  Window window(1280, 800, "Volume Rendering");
#endif
  if (!window.get())
  {
    std::cerr << "Can't open GLFW window" << std::endl;
    exit(1);
  }

  // �X���C�X�̍쐬
  const GLuint vao(makeSlice());

  // 3D �e�N�X�`���̍쐬
  const GLuint vtex(makeVolume(texWidth, texHeight, texDepth, texPattern));

  // 3D �e�N�X�`���̌��z�e�N�X�`���̍쐬
  const GLuint gtex(gradient(vao, vtex, texWidth, texHeight, texDepth));

  // �V�F�[�_
  const GLuint program(ggLoadShader("slice.vert", "slice.frag"));
  const GLint mtLoc(glGetUniformLocation(program, "mt"));
  const GLint mcLoc(glGetUniformLocation(program, "mc"));
  const GLint spacingLoc(glGetUniformLocation(program, "spacing"));
  const GLint vtexLoc(glGetUniformLocation(program, "vtex"));
  const GLint gtexLoc(glGetUniformLocation(program, "gtex"));
  const GLint thresholdLoc(glGetUniformLocation(program, "threshold"));

  // �B�ʏ����͍s��Ȃ�
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // �A���t�@�u�����f�B���O�̕��@��ݒ肷��
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // �w�i�F��ݒ肷��
  glClearColor(back[0], back[1], back[2], back[3]);

  // �E�B���h�E���J���Ă���Ԃ���Ԃ��`�悷��
  while (!window.shouldClose())
  {
    // ��ʃN���A
    window.clear();

    // �V�F�[�_�̎g�p
    glUseProgram(program);
    glUniformMatrix4fv(mtLoc, 1, GL_TRUE, window.getTb().get());
    glUniform1f(spacingLoc, 1.0f / GLfloat(slices - 1));
    glUniform1i(vtexLoc, 0);
    glUniform1i(gtexLoc, 1);
    glUniform1f(thresholdLoc, window.getThreshold());

    // 3D �e�N�X�`���̃}�b�s���O
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, vtex);

    // 3D �e�N�X�`���̌��z�e�N�X�`���̃}�b�s���O
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, gtex);

    // �X���C�X�̕`��
#if STEREO == NONE
    glUniformMatrix4fv(mcLoc, 1, GL_FALSE, window.getMc().get());
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, slices);
#else
    glUniformMatrix4fv(mcLoc, 1, GL_FALSE, window.getMcL().get());
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, slices);
    glUniformMatrix4fv(mcLoc, 1, GL_FALSE, window.getMcR().get());
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, slices);
#endif
    // �_�u���o�b�t�@�����O
    window.swapBuffers();
  }

  return 0;
}
