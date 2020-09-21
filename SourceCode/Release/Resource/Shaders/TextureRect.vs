#version 330

in vec3 a_Position;
in vec2 a_TexPos;
uniform vec4 u_Trans;
out vec2 v_TexPos;

void main()
{
	vec4 newPosition;
	newPosition.xy = a_Position.xy*u_Trans.zw + u_Trans.xy;
	newPosition.z = 0;
	newPosition.w= 1;
	gl_Position = newPosition;
	v_TexPos = a_TexPos;
}
