#pragma once
//
// �ꎟ���̃m�C�Y�֐�
//

// 3 �� Catmull-Rom Spline ���
extern double catmull_rom(double x0, double x1, double x2, double x3, double t);

// �ꎟ���̃m�C�Y�֐�
class Noise1
{
  int n;                                          // �ߓ_��
  double *p;                                      // �e�ߓ_�̒l
  void copy(const Noise1 &noise);                 // �f�[�^�̃R�s�[

public:

  // �R���X�g���N�^
  Noise1() { p = 0; }
  Noise1(int n);
  Noise1(const Noise1 &noise) { copy(noise); }

  // �f�X�g���N�^
  virtual ~Noise1() { delete[] p; }

  // ������Z�q
  Noise1 &operator=(const Noise1 &noise);

  // �ߓ_ i �̒l�𓾂�
  double point(int i) const { return p[i]; }

  // x �ɂ������Ԓl�����߂� (0 �� x �� n)
  double noise(double x) const;

  // x �ɂ����� o �I�N�^�[�u�� Perlin �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
  double perlin(double x, int o, double a) const;

  // x �ɂ����� o �I�N�^�[�u�� Turbulence �m�C�Y�����߂� (�I�N�^�[�u�Ԃ̌����� a)
  double turbulence(double x, int o, double a) const;
};
