#version 330

uniform mat4 projMat, viewMat, modelMat;
uniform mat3 NIT = mat3(1.0);

in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;   // Currently not used. You can use it to replace or combine with the diffuse component of the material
in vec2 vTexCoord1;
in vec2 vTexCoord2;


// Surfel: a SURFace ELement. All coordinates are in world space
out vec3 surfel_position;
out vec3 surfel_normal;
out vec4 surfel_color;
out vec2 surfel_texCoord1;
out vec2 surfel_texCoord2;

out vec3 cameraPosition;

void main()
{
    // All attributes are in world space
    surfel_position = vec3(modelMat*vec4(vPosition,1.0f));
    surfel_normal = normalize(NIT * vNormal);
//    surfel_color  = vColor;
    surfel_color  = vec4(1.0, 1.0, 1.0, 1.0);
    surfel_texCoord1 = vTexCoord1;
    surfel_texCoord2 = vTexCoord2;

    // Compute the position of the camera in world space
    cameraPosition = - vec3(viewMat[3]) * mat3(viewMat);

    // Define the fragment position on the screen
    gl_Position = projMat*viewMat*vec4(surfel_position,1.0f);
}
