#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// Oculus Rift �̃A�X�y�N�g��
uniform float ocuAspect;

// Oculus Rift �̃����Y�̒��S�̉�ʂ̒��S����̂���
uniform float projectionCenterOffset;

// ���_����
layout (location = 0) in vec4 pv;

// ���X�^���C�U�ɑ��钸�_����
out vec2 t;

void main()
{
	t = vec2(pv.x * ocuAspect + projectionCenterOffset, pv.y);
	gl_Position = pv;
}
