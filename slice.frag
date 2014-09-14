#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// ����
const vec4 lamb = vec4(0.4, 0.4, 0.4, 1.0);         // ��������
const vec4 ldiff = vec4(0.8, 0.8, 0.8, 1.0);        // �g�U���ˌ�����
const vec4 lspec = vec4(0.8, 0.8, 0.8, 1.0);        // ���ʔ��ˌ�����
const vec4 pl = vec4(0.0, 0.5, 1.0, 0.0);           // �ʒu

// �ގ�
const vec4 kamb = vec4(0.8, 0.8, 0.4, 1.0);         // �����̔��ˌW��
const vec4 kdiff = vec4(0.6, 0.6, 0.3, 1.0);        // �g�U���ˌW��
const vec4 kspec = vec4(0.4, 0.4, 0.2, 1.0);        // ���ʔ��ˌW��
const float kshi = 50.0;                               // �P���W��

// �e�N�X�`���̃T���v��
uniform sampler3D volume;

// �e�N�X�`�����W�̕ϊ��s��
uniform mat4 mt;

// 臒l
uniform float threshold;

// �X���C�X�̕\�ʏ�̈ʒu
in vec4 p;

// �e�N�X�`�����W
in vec3 t;

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 fc;

void main()
{
  // ���̃{�����[���̔Z�x��臒l�̍�
  float v = texture(volume, t).r - threshold;

  // �Z�x��臒l�ȉ��Ȃ�t���O�����g���̂Ă�
  if (v <= 0.0) discard;

  // �Z�x�̌��z�����߂�
  vec3 g = vec3(
    textureOffset(volume, t, ivec3(-1, 0, 0)).r - textureOffset(volume, t, ivec3(1, 0, 0)).r,
    textureOffset(volume, t, ivec3(0, -1, 0)).r - textureOffset(volume, t, ivec3(0, 1, 0)).r,
    textureOffset(volume, t, ivec3(0, 0, -1)).r - textureOffset(volume, t, ivec3(0, 0, 1)).r
  );

#if 1
  vec3 l = normalize((pl * p.w - p * pl.w).xyz);  // �����x�N�g��
  vec3 n = normalize(g * mat3(mt));               // �@���x�N�g��
  vec3 h = normalize(l - normalize(p.xyz));       // ���ԃx�N�g��

  // �g�U���ˌ��{�����̔��ˌ�
  vec4 idiff = max(dot(n, l), 0.0) * kdiff * ldiff + kamb * lamb;

  // ���ʔ��ˌ�
  vec4 ispec = pow(max(dot(n, h), 0.0), kshi) * kspec * lspec;

  // �t���O�����g�̐F
  fc = vec4((idiff + ispec).rgb, v);
#else
  // ���z�����̂܂܃t���O�����g�̐F�Ɏg��
  fc = vec4(normalize(g) * 0.5 + 0.5, v);
#endif
}
