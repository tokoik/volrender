#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// ����
const vec4 lamb = vec4(0.4f, 0.4f, 0.4f, 1.0f);         // ��������
const vec4 ldiff = vec4(0.8f, 0.8f, 0.8f, 1.0f);        // �g�U���ˌ�����
const vec4 lspec = vec4(0.8f, 0.8f, 0.8f, 1.0f);        // ���ʔ��ˌ�����
const vec4 pl = vec4(0.0f, 0.5f, 1.0f, 0.0f);           // �ʒu

// �ގ�
const vec4 kamb = vec4(0.8f, 0.8f, 0.4f, 1.0f);         // �����̔��ˌW��
const vec4 kdiff = vec4(0.6f, 0.6f, 0.3f, 1.0f);        // �g�U���ˌW��
const vec4 kspec = vec4(0.4f, 0.4f, 0.2f, 1.0f);        // ���ʔ��ˌW��
const float kshi = 50.0f;                               // �P���W��

// �e�N�X�`���̃T���v��
uniform sampler3D volume;

// �e�N�X�`�����W�̕ϊ��s��
uniform mat4 mt;

// �N���b�s���O���W�n�ւ̕ϊ��s��
uniform mat4 mc;

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
  vec4 g = vec4(
    textureOffset(volume, t, ivec3(-1, 0, 0)).r - textureOffset(volume, t, ivec3(1, 0, 0)).r,
    textureOffset(volume, t, ivec3(0, -1, 0)).r - textureOffset(volume, t, ivec3(0, 1, 0)).r,
    textureOffset(volume, t, ivec3(0, 0, -1)).r - textureOffset(volume, t, ivec3(0, 0, 1)).r,
    0.0
  );

#if 1
  vec3 e = normalize((p * mc[3].w - mc[3] * p.w).xyz);  // �����x�N�g��
  vec3 l = normalize((pl * p.w - p * pl.w).xyz);        // �����x�N�g��
  vec3 n = normalize((g * mt).xyz);                     // �@���x�N�g��
  vec3 h = normalize(l - e);                            // ���ԃx�N�g��

  // �g�U���ˌ��{�����̔��ˌ�
  vec4 idiff = max(dot(n, l), 0.0) * kdiff * ldiff + kamb * lamb;

  // ���ʔ��ˌ�
  vec4 ispec = pow(abs(dot(n, h)), kshi) * kspec * lspec;

  // �t���O�����g�̐F
  fc = vec4((idiff + ispec).rgb, v);
#else
  // ���z�����̂܂܃t���O�����g�̐F�Ɏg��
  fc = vec4(g.rgb, v);
#endif
}
