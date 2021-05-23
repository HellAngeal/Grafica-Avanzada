#version 330 core

out vec4 fragColor;

in vec3 nuestroColor;
in vec2 nuestraTextura;

//textura
uniform sampler2D textura1;
uniform sampler2D textura2;


void main()
{
	fragColor = mix(texture(textura1, nuestraTextura), texture(textura2, nuestraTextura), 0.4);
}