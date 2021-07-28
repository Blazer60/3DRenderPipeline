#shader vertex
#version 460 core

//     ws - World Space.
//     ts - Tangent Space.
//     cs - Camera Space.


layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coord;
layout(location = 2) in vec4 normal;
layout(location = 3) in vec4 tangent;
layout(location = 4) in vec4 bi_tangent;
layout(location = 5) in int  texture_id;


out      vec2 v_texture_coord;
out flat int  f_texture_id;
out      vec4 v_vertex_position_ts;
out      vec4 v_vertex_position_ws;
out      vec4 v_camera_position_ts;
out      mat4 v_tbn_matrix;

uniform mat4 u_mvp_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

uniform vec4 u_camera_position_ws;


mat4 create_tbn_matrix()
{
    const mat4 mv_matrix = u_view_matrix * u_model_matrix;

    const vec4 normal_cs     = mv_matrix * vec4(normalize(normal.xyz),     0.0);
    const vec4 tangent_cs    = mv_matrix * vec4(normalize(tangent.xyz),    0.0);
    const vec4 bi_tangent_cs = mv_matrix * vec4(normalize(bi_tangent.xyz), 0.0);


    return transpose(mat4(
        tangent_cs,
        bi_tangent_cs,
        normal_cs,
        vec4(0.0, 0.0, 0.0, 0.0)
    ));
}

void main()
{
    v_texture_coord = texture_coord;
    f_texture_id    = texture_id;

    v_tbn_matrix = create_tbn_matrix();

    v_vertex_position_ts = v_tbn_matrix * u_view_matrix * u_model_matrix * position;
    v_vertex_position_ws = u_model_matrix * position;
    v_camera_position_ts = v_tbn_matrix * u_view_matrix * u_camera_position_ws;

    gl_Position = u_mvp_matrix * position;
}


#shader fragment
#version 460 core


struct PointLight
{
    vec4  position_ws;
    vec4  colour;
    float intensity;
    float fall_off;
};

struct Material
{
    vec4  k_ambient;
    vec4  k_diffuse;
    vec4  k_specular;
    float n_specular;
};


in      vec2 v_texture_coord;
in flat int  f_texture_id;
in      vec4 v_vertex_position_ts;
in      vec4 v_vertex_position_ws;
in      vec4 v_camera_position_ts;
in      mat4 v_tbn_matrix;


uniform sampler2DArray u_diffuse_map_textures;
uniform sampler2DArray u_normal_map_textures;

uniform mat4 u_view_matrix;

uniform PointLight u_lights    [32];
uniform Material   u_materials [32];

vec4 get_light_intensity(PointLight light)
{
    const float d         = distance(v_vertex_position_ws, light.position_ws);
    const float intensity = light.intensity * (light.fall_off / (d * d));

    return light.colour * intensity;
}

vec4 calculate_ambient(PointLight light)
{
    return light.colour * light.intensity * 0.2;
}

float calculate_diffuse_power(vec4 light_direction_ts, vec4 texture_normal_ts)
{
    return max(dot(light_direction_ts, texture_normal_ts), 0.0);
}

float calculate_specular_power(vec4 light_direction_ts, vec4 texture_normal_ts)
{
    const float exponent = u_materials[f_texture_id].n_specular;
    if (exponent <= 0.0) { return 0; }

    const vec4 view_direction_ts  = vec4(normalize(v_camera_position_ts.xyz - v_vertex_position_ts.xyz), 0.0);
    const vec4 half_angle         = normalize(light_direction_ts + view_direction_ts);

    return max(pow(dot(half_angle, texture_normal_ts), exponent), 0.0);
}

void main()
{
    const vec4 texture_normal    = texture(u_normal_map_textures, vec3(v_texture_coord, f_texture_id));
    const vec4 texture_normal_ts = normalize(vec4(2.0, 2.0, 2.0, 0.0) * texture_normal - vec4(1.0, 1.0, 1.0, 0.0));

    vec4 k_light_ambient  = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 k_light_diffuse  = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 k_light_specular = vec4(0.0, 0.0, 0.0, 1.0);

    for (int i = 0; i < 1; ++i)
    {
        PointLight light              = u_lights[i];
        const vec4 light_position_ts  = v_tbn_matrix * u_view_matrix * light.position_ws;
        const vec4 light_direction_ts = vec4(normalize(light_position_ts.xyz - v_vertex_position_ts.xyz), 0.0);
        const vec4 intensity          = get_light_intensity(light);

        k_light_ambient  += calculate_ambient(light);
        k_light_diffuse  += intensity * calculate_diffuse_power  (light_direction_ts, texture_normal_ts);
        k_light_specular += intensity * calculate_specular_power (light_direction_ts, texture_normal_ts);
    }

    vec4 k_diffuse_texture_colour = texture(u_diffuse_map_textures, vec3(v_texture_coord, f_texture_id));

    vec4 k_base_ambient  = u_materials[f_texture_id].k_ambient;
    vec4 k_base_diffuse  = u_materials[f_texture_id].k_diffuse;
    vec4 k_base_specular = u_materials[f_texture_id].k_specular;

    gl_FragColor = k_base_ambient  * k_light_ambient  * k_diffuse_texture_colour +
                   k_base_diffuse  * k_light_diffuse  * k_diffuse_texture_colour +
                   k_base_specular * k_light_specular;
}
