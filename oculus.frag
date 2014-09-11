#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// �e�N�X�`��
uniform sampler2D ocuFboColor;

// �����Y�̘c�݂̕␳�W��
uniform vec4 lensDistortion;

// �����Y�̊g��k�̕␳�W��
uniform float lensScale;

// ���X�^���C�U����󂯎�钸�_�����̕�Ԓl
in vec2 t;                                          // �e�N�X�`�����W

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 fc;                  // �t���O�����g�̐F

void main()
{
  vec4 r;
  r.x = 1.0;
  r.y = dot(t, t);	// r^2
  r.z = r.y * r.y;	// r^4
  r.w = r.y * r.z;	// r^6
#if 1
  vec2 tc = t * dot(r, lensDistortion);
  fc = texture(ocuFboColor, tc * lensScale * 0.5 + 0.5);
#else
  vec2 tr = t * dot(r, lensDistortion);
  vec2 tg = t * dot(r, lensDistortion + vec4(0.0, 0.05, 0.04, 0.0));
  vec2 tb = t * dot(r, lensDistortion + vec4(0.0, 0.1, 0.08, 0.0));
  fc.r = texture(ocuFboColor, tr * lensScale * 0.5 + 0.5).r;
  fc.g = texture(ocuFboColor, tg * lensScale * 0.5 + 0.5).g;
  fc.b = texture(ocuFboColor, tb * lensScale * 0.5 + 0.5).b;
  fc.a = 1.0;
#endif
}
