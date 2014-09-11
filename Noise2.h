#pragma once
//
// �񎟌��̃m�C�Y�֐�
//

#include "Noise1.h"

// �񎟌��̃m�C�Y�֐�
class Noise2
{
  int m;                                          // �ߓ_��
  Noise1 **p;                                     // �e�ߓ_�̒l
  void erase();                               // �f�[�^�̏���
  void copy(const Noise2 &noise);                 // �f�[�^�̃R�s�[

public:

  // �R���X�g���N�^
  Noise2() { p = 0; }
  Noise2(int n, int m);
  Noise2(const Noise2 &noise) { copy(noise); }

  // �f�X�g���N�^
  virtual ~Noise2() { erase(); }

  // ������Z�q
  Noise2 &operator=(const Noise2 &noise);

  // �ߓ_ (i, j) �̒l�𓾂�
  double point(int i, int j) const { return p[j]->point(i); }

  // (x, y) �ɂ������Ԓl�����߂� (0 �� x �� n, 0 �� y �� m)
  double noise(double x, double y) const;

  // (x, y) �ɂ����� o �I�N�^�[�u�� Perlin �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
  double perlin(double p, double q, int o, double a) const;

  // (x, y) �ɂ����� o �I�N�^�[�u�� Turbulence �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
  double turbulence(double x, double y, int o, double a) const;
};
