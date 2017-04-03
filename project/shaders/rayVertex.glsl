#version 330 //GLSL version, fit with OpenGL version

in vec2 vPosition;

out vec2 pixelCoords;

void main()
{
    // Transform coordinates from local space to clipped space
    gl_Position = vec4(vPosition, 0.0f, 1.0f);
    pixelCoords = vPosition;
}