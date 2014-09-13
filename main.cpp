// 標準ライブラリ
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

// ノイズ
#include "Noise3.h"

// ウィンドウ関連の処理
#include "Window.h"

namespace
{
  //
  // スライスの作成
  //
  GLuint makeSlice()
  {
    // 頂点配列オブジェクトを作成して結合する
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 頂点バッファオブジェクトを作成して結合する
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // バッファオブジェクトを確保する
    static const GLfloat p[] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    glBufferData(GL_ARRAY_BUFFER, sizeof p, p, GL_STATIC_DRAW);

    // 頂点位置は index == 0 の in 変数から得る
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    return vao;
  }

  //
  // ボリュームデータの作成
  //
  GLuint makeVolume(GLint width, GLint height, GLint depth)
  {
    // 作業用メモリ
    std::vector<GLubyte> texture;

    // ノイズ関数を初期化する
    const Noise3 noise(5, 5, 5);

    // ノイズ関数を使ってテクスチャを作る
    for (GLint k = 0; k < depth; ++k)
    {
      const double z(double(k) / double(depth - 1));

      for (GLint j = 0; j < height; ++j)
      {
        const double y(double(j) / double(height - 1));

        for (GLint i = 0; i < width; ++i)
        {
          const double x(double(i) / double(width - 1));

#if VOLUMETYPE == CHECKER
          texture.push_back(((i >> 4) + (j >> 4) + (k >> 4)) & 1 ? 0 : 255);
#elif VOLUMETYPE == NOISE
          texture.push_back(GLubyte(noise.noise(x, y, z) * 255.0));
#elif VOLUMETYPE == PERLIN
          texture.push_back(GLubyte(noise.perlin(x, y, z, 4, 0.5) * 255.0));
#elif VOLUMETYPE == TURBULENCE
          texture.push_back(GLubyte(noise.turbulence(x, y, z, 4, 0.5) * 255.0));
#elif VOLUMETYPE == SPHERE
          const double px(2.0 * x - 1.0), py(2.0 * y - 1.0), pz(2.0 * z - 1.0);
          texture.push_back(GLubyte(255.0 - sqrt(px * px + py * py + pz * pz) * 127.5));
#else
          texture.push_back(255);
#endif
        }
      }
    }

    // テクスチャオブジェクトを作成して結合する
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_3D, tex);

    // テクスチャを割り当てる
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, width, height, depth, 0,
      GL_RED, GL_UNSIGNED_BYTE, &texture[0]);

    // テクスチャの拡大・縮小に線形補間を用いる
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // テクスチャからはみ出た部分には境界色を用いる
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    // テクスチャの境界色を設定する (ボリュームの外には何もない)
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, black);

    return tex;
  }

  //
  // 終了処理
  //
  void cleanup()
  {
    // GLFW の終了処理
    glfwTerminate();
  }
}

//
// メインプログラム
//
int main(int argc, const char * argv[])
{
  // GLFW を初期化する
  if (glfwInit() == GL_FALSE)
  {
    // GLFW の初期化に失敗した
    std::cerr << "Can't initialize GLFW" << std::endl;
    return EXIT_FAILURE;
  }

  // プログラム終了時の処理を登録する
  atexit(cleanup);

#if STEREO == OCULUS
  // Oculus Rift (LibOVR) を初期化する
  System::Init(Log::ConfigureDefaultLog(LogMask_All));
#endif

  // OpenGL Version 3.2 Core Profile を選択する
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_STEREO, STEREO == QUADBUFFER);

#if STEREO != NONE && !defined(_DEBUG)
  // 接続されているモニタを捜す
  int mcount;
  GLFWmonitor **const monitors = glfwGetMonitors(&mcount);

  // セカンダリモニタがあればそれを使う
  GLFWmonitor *const monitor(monitors[mcount > 1 ? useSecondary : 0]);

  // モニタのモードを調べる
  const GLFWvidmode* mode(glfwGetVideoMode(monitor));

  // フルスクリーンでウィンドウを開く
  Window window(mode->width, mode->height, "Volume Rendering (STEREO)", monitor);
#else
  // ウィンドウモードでウィンドウを開く
  Window window(1280, 800, "Volume Rendering");
#endif
  if (!window.get())
  {
    std::cerr << "Can't open GLFW window" << std::endl;
    exit(1);
  }

  // スライスの作成
  const GLuint slice(makeSlice());

  // 3D テクスチャの作成
  const GLuint volume(makeVolume(texWidth, texHeight, texDepth));

  // シェーダ
  const GLuint program(ggLoadShader("slice.vert", "slice.frag"));
  const GLint volumeLoc(glGetUniformLocation(program, "volume"));
  const GLint mtLoc(glGetUniformLocation(program, "mt"));
  const GLint mwLoc(glGetUniformLocation(program, "mw"));
  const GLint mpLoc(glGetUniformLocation(program, "mp"));
  const GLint spacingLoc(glGetUniformLocation(program, "spacing"));
  const GLint thresholdLoc(glGetUniformLocation(program, "threshold"));

  // 隠面消去は行わない
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  // アルファブレンディングの方法を設定する
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // 背景色を設定する
  glClearColor(back[0], back[1], back[2], back[3]);

  // ウィンドウが開いている間くり返し描画する
  while (!window.shouldClose())
  {
    // 画面クリア
    window.clear();

    // シェーダの使用
    glUseProgram(program);
    glUniform1i(volumeLoc, 0);
    glUniformMatrix4fv(mtLoc, 1, GL_TRUE, window.getMt().get());
    glUniform1f(spacingLoc, 1.0f / GLfloat(slices - 1));
    glUniform1f(thresholdLoc, window.getThreshold());

    // 3D テクスチャのマッピング
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, volume);

    // スライスの描画
    glBindVertexArray(slice);
#if STEREO == NONE
    glUniformMatrix4fv(mwLoc, 1, GL_FALSE, window.getMw().get());
    glUniformMatrix4fv(mpLoc, 1, GL_FALSE, window.getMp().get());
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, slices);
#else
    glUniformMatrix4fv(mwLoc, 1, GL_FALSE, window.getMwL().get());
    glUniformMatrix4fv(mpLoc, 1, GL_FALSE, window.getMpL().get());
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, slices);
    glUniformMatrix4fv(mwLoc, 1, GL_FALSE, window.getMwR().get());
    glUniformMatrix4fv(mpLoc, 1, GL_FALSE, window.getMpR().get());
    glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, slices);
#endif

    // ダブルバッファリング
    window.swapBuffers();
  }

  return 0;
}
