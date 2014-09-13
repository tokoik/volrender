#pragma once

//
// 各種設定
//

// 補助プログラム
#include "gg.h"
using namespace gg;

// 3D テクスチャのパターン
#define CHECKER       0
#define SPHERE        1
#define NOISE         2
#define PERLIN        3
#define TURBULENCE    4

// 使用する 3D テクスチャのパターン
#define VOLUMETYPE    TURBULENCE

// 3D テクスチャのサイズ
const GLsizei texWidth(32);
const GLsizei texHeight(32);
const GLsizei texDepth(32);

// スライス数
const int slices(256);

// 立体視の設定
#define NONE          0                                 // 単眼視
#define LINEBYLINE    1                                 // インターレース（未サポート）
#define TOPANDBOTTOM  2                                 // 上下
#define SIDEBYSIDE    3                                 // 左右
#define QUADBUFFER    4                                 // クワッドバッファステレオ
#define OCULUS        5                                 // Oculus Rift (HMD)

// 立体視の方式
#define STEREO        NONE

// 立体視特有のパラメータ
#if STEREO != NONE
const int useSecondary(1);                              // 1 ならセカンダリモニタに表示
const GLfloat initialParallax(0.032f);                  // 視差の初期値 (単位 m)
const GLfloat parallaxStep(0.001f);                     // 視差の変更ステップ (単位 m)
const GLfloat screenDistance(2.0f);                     // 実際のスクリーンまでの距離 (単位 m)

// Oculus Rift 固有のパラメータ
#  if STEREO == OCULUS
const GLfloat lensScaleStep(0.001f);                    // レンズの拡大率の補正係数の調整ステップ
const GLfloat projectionCenterOffsetStep(0.001f);       // レンズの中心位置の調整ステップ
const GLuint fboWidth(1024), fboHeight(1024);           // 補正に使う FBO のサイズ
#  endif
#endif

// カメラの初期状態
const GLfloat startPosition[] = { 0.0f, 0.0f, -2.0f };  // ボリュームデータの中心の初期位置
const GLfloat displayCenter(0.5f);                      // ディスプレイの中心位置 (高さの半分
const GLfloat displayDepth(1.5f);                       // 観測者とディスプレイ面との距離
const GLfloat zNear(0.1f);                              // 前方面までの距離
const GLfloat zFar(10.0f);                              // 後方面までの距離

// ナビゲーションの速度調整
const GLfloat speedScale(0.00005f);                     // フレームあたりの移動速度係数
const GLfloat threasholdStep(0.01f);                    // 閾値の調整係数
const GLfloat axesSpeedScale(0.01f);                    // ゲームパッドのスティックの速度の係数
const GLfloat axesAngleScale(0.03f);                    // ゲームパッドのスティックの角速度の係数
const GLfloat btnsScale(0.005f);                        // ゲームパッドのボタンの係数

// 境界色 (Oculus Rift 表示時の表示範囲外の色)
const GLfloat border[] = { 0.0, 0.0, 0.0, 0.0 };

// 背景色
const GLfloat back[] = { 0.2f, 0.3f, 0.4f, 0.0f };
