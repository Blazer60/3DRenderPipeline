#shader vertex
#version 460 core


layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_coord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;
layout(location = 5) in int texture_id;

uniform mat3 u_mvp;
uniform mat3 u_viewMat;
uniform mat3 u_modelMat;
uniform vec3 u_cameraPosition;

out vec2 v_texture_coord;
out vec4 v_vertWorldPos;
out vec3 v_normal;
out flat int v_texture_id;
out vec3 v_camera_position_ts;


void main()
{
    gl_Position.xyz = u_mvp * position;

    vec3 normalCameraSpace      = u_viewMat * u_modelMat * normalize(normal);
    vec3 tangentCameraSpace     = u_viewMat * u_modelMat * normalize(tangent);
    vec3 biTangentCameraSpace   = u_viewMat * u_modelMat * normalize(biTangent);

    mat3 tbnMat = transpose(mat3(
        tangentCameraSpace,
        biTangentCameraSpace,
        normalCameraSpace
    ));

    v_camera_position_ts = tbnMat * u_cameraPosition;
    v_normal = normal;
    v_vertWorldPos.xyz = u_modelMat * position;
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
uniform vec3 u_cameraPosition;
uniform sampler2DArray u_kDiffuseTextures;
uniform sampler2DArray u_normalMapTextures;

in vec3 v_normal;
in vec2 v_texture_coord;
in vec4 v_vertWorldPos;
in flat int v_texture_id;
in vec3 v_camera_position_ts;


void main()  // line 32
{
    vec4 ambientColour = vec4(0.0);
    vec4 diffuseColour = vec4(0.0);
    vec4 specularColour = vec4(0.0);
    float exponent = u_materials[v_texture_id].nSpecular;

    vec4 texture_normal_ts = 0.5 * texture(u_normalMapTextures, vec3(v_texture_coord, v_texture_id)) + 0.5;


    for (int i = 0; i < 1; ++i)
    {
        PointLight light = u_lights[i];
        // Ambient Lighting.
        ambientColour += light.colour * light.intensity * 0.2;

        // Diffuse Lighting.
        float d = distance(v_vertWorldPos, light.position);
        float intensity = light.intensity * (light.fallOff / (d * d));
        float angle = max(dot(vec4(v_normal, 1.0), (light.position - v_vertWorldPos)), 0.0);
        diffuseColour += intensity * angle * light.colour;

        // Specular Lighting.
        vec4 lightDirection = normalize(light.position - v_vertWorldPos);
        vec4 viewDirection = normalize(vec4(u_cameraPosition, 1.0) - v_vertWorldPos);
        vec4 halfAngle = normalize(lightDirection + viewDirection);
        float power = max(pow(dot(halfAngle, vec4(v_normal, 1.0)), exponent), 0.0);
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
