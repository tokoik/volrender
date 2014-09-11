#pragma once
//
// �O�����̃m�C�Y�֐�
//

#include "Noise2.h"

// �O�����̃m�C�Y�֐�
class Noise3
{
  int l;                                          // �ߓ_��
  Noise2 **p;                                     // �e�ߓ_�̒l
  void erase();                               // �f�[�^�̏���
  void copy(const Noise3 &noise);                 // �f�[�^�̃R�s�[

public:

  // �R���X�g���N�^
  Noise3() { p = 0; }
  Noise3(int n, int m, int l);
  Noise3(const Noise3 &noise) { copy(noise); }

  // �f�X�g���N�^
  virtual ~Noise3() { erase(); }

  // ������Z�q
  Noise3 &operator=(const Noise3 &noise);

  // �ߓ_ (i, j, k) �̒l�𓾂� 
  double point(int i, int j, int k) const { return p[k]->point(i, j); }

  // (x, y, z) �ɂ������Ԓl�����߂� (0 �� x �� n, 0 �� y �� m, 0 �� z �� l)
  double noise(double x, double y, double z) const;

  // (x, y, z) �ɂ����� o �I�N�^�[�u�� Perlin �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
  double perlin(double x, double y, double z, int o, double a) const;

  // (x, y, z) �ɂ����� o �I�N�^�[�u�� Turbulence �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
  double turbulence(double x, double y, double z, int o, double a) const;
};
