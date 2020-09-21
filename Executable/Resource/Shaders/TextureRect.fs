#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_TexPos;
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec2 newTexPos = vec2(v_TexPos.x, 1-v_TexPos.y);
	FragColor = texture(u_Texture, newTexPos)*u_Color;
}
