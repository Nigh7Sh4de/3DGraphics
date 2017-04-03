#version 330

uniform mat4 projMat, viewMat, modelMat;

// This is the normal inverse transpose matrix.
// It is really important to obtain a normal in world coordinates.
// It should NEVER BE COMPUTED IN THE VERTEX SHADER, since an inverse computation
// is quite expensive and the result is the same for ALL vertices.
uniform mat3 NIT = mat3(1.0);

// Attributes
in vec3 vPosition;
in vec4 vColor;   // Currently not used. You can use it to replace or combine with the diffuse component of the material
in vec3 vNormal;

// Surfel: a SURFace ELement. All coordinates are in world space
out vec3 surfel_position;
out vec3 surfel_normal;
out vec4 surfel_color;

out vec3 camera_position;

void main()
{
    // Surfel attributes in world space
    surfel_position = vec3(modelMat*vec4(vPosition, 1.0f));
    surfel_normal = normalize(NIT * vNormal);
    surfel_color = vColor;

    // Camera position in world space
    camera_position = - vec3(viewMat[3]) * mat3(viewMat);

    // Fragment position on screen
    gl_Position = projMat*viewMat*vec4(surfel_position,1.0f);
}
