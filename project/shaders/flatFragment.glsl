#version 330 //GLSL version, fit with OpenGL version
in vec4 fragmentColor;
out vec4 outColor;  // resulting color of the fragment shader

void main()
{
    outColor = fragmentColor;
}
