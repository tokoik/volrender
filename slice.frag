#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// ����
const vec4 lamb = vec4(0.4f, 0.4f, 0.4f, 1.0f);     // ��������
const vec4 ldiff = vec4(0.8f, 0.8f, 0.8f, 1.0f);    // �g�U���ˌ�����
const vec4 lspec = vec4(0.8f, 0.8f, 0.8f, 1.0f);    // ���ʔ��ˌ�����
const vec4 pl = vec4(0.0f, 0.5f, 1.0f, 0.0f);       // �ʒu

// �ގ�
const vec4 kamb = vec4(0.8f, 0.8f, 0.4f, 1.0f);     // �����̔��ˌW��
const vec4 kdiff = vec4(0.8f, 0.8f, 0.4f, 1.0f);    // �g�U���ˌW��
const vec4 kspec = vec4(0.2f, 0.2f, 0.2f, 1.0f);    // ���ʔ��ˌW��
const float kshi = 50.0f;                           // �P���W��

// �e�N�X�`���̃T���v��
uniform sampler3D vtex;

// �e�N�X�`���̌��z�e�N�X�`���̃T���v��
uniform sampler3D gtex;

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
  float v = texture(vtex, t).a - threshold;

  // �Z�x��臒l��������Ă�����t���O�����g���̂Ă�
  if (v < 0.0) discard;

  // �{�����[���̌��z
  vec4 g = texture(gtex, t);

#if 1
  vec3 e = normalize(p.xyz / p.w);                  // �����x�N�g��
  vec3 l = normalize((pl * p.w - p * pl.w).xyz);    // �����x�N�g��
  vec3 n = normalize(((g * 2.0 - 1.0) * mt).xyz);   // �@���x�N�g��
  vec3 h = normalize(l - e);                        // ���ԃx�N�g��

  vec4 idiff = max(dot(n, l), 0.0) * kdiff * ldiff + kamb * lamb;
  vec4 ispec = pow(max(dot(n, h), 0.0), kshi) * kspec * lspec;
  fc = vec4((idiff + ispec).rgb, v);
#else
  // ���z�����̂܂܃t���O�����g�̐F�Ɏg��
  fc = vec4(g.rgb, v);
#endif
}
