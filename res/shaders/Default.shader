#shader vertex
#version 460 core


layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coord;
layout(location = 3) in int texture_id;

uniform mat4 u_mvp;
uniform mat4 u_viewMat;
uniform mat4 u_modelMat;

out vec2 v_texture_coord;
out vec4 v_vertWorldPos;
out vec4 v_normal;
out flat int v_texture_id;


void main()
{
    gl_Position = u_mvp * position;

    v_normal = normal;
    v_vertWorldPos = u_modelMat * position;
    v_texture_coord = texture_coord;
    v_texture_id = texture_id;
}

#shader fragment
#version 460 core

struct PointLight
{
  vec4 position;
  vec4 colour;
  float intensity;
  float fallOff;
};

struct Material
{
    vec4 kAmbient;
    vec4 kDiffuse;
    vec4 kSpecular;
    float nSpecular;
};

uniform Material u_materials[32];
uniform PointLight u_lights[32];
uniform vec4 u_cameraPosition;
uniform sampler2DArray u_kDiffuseTextures;

in vec4 v_normal;
in vec2 v_texture_coord;
in vec4 v_vertWorldPos;
in flat int v_texture_id;


void main()
{
    vec4 ambientColour = vec4(0.0);
    vec4 diffuseColour = vec4(0.0);
    vec4 specularColour = vec4(0.0);
    float exponent = u_materials[v_texture_id].nSpecular;
    for (int i = 0; i < 1; ++i)
    {
        PointLight light = u_lights[i];
        // Ambient Lighting.
        ambientColour += light.colour * light.intensity * 0.2;

        // Diffuse Lighting.
        float d = distance(v_vertWorldPos, light.position);
        float intensity = light.intensity * (light.fallOff / (d * d));
        float angle = max(dot(v_normal, (light.position - v_vertWorldPos)), 0.0);
        diffuseColour += intensity * angle * light.colour;

        // Specular Lighting.
        vec4 lightDirection = normalize(light.position - v_vertWorldPos);
        vec4 viewDirection = normalize(u_cameraPosition - v_vertWorldPos);
        vec4 halfAngle = normalize(lightDirection + viewDirection);
        float power = max(pow(dot(halfAngle, v_normal), exponent), 0.0);
        specularColour += light.colour * intensity * power;
    }
    diffuseColour = clamp(diffuseColour, vec4(0.0), vec4(1.0));
    diffuseColour.w = 1.0;
    specularColour.w = 1.0;

    vec4 kDiffuseTexture = texture(u_kDiffuseTextures, vec3(v_texture_coord, v_texture_id));

    vec4 kAmbient = u_materials[v_texture_id].kAmbient;
    vec4 kDiffuse = u_materials[v_texture_id].kDiffuse;
    vec4 kSpecular = u_materials[v_texture_id].kSpecular;

    gl_FragColor = kAmbient * kDiffuseTexture * ambientColour +
                   kDiffuse * kDiffuseTexture * diffuseColour +
                   kSpecular                  * specularColour;
}
