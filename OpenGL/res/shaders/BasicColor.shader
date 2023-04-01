#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal; ;
    gl_Position = projection * view * model * position;
    FragPos = vec3(model * position);
};

#shader fragment
#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

layout(location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec4 objectColor;
uniform vec4 lightColor;
uniform vec3 viewPos;

void main()
{
    if (Normal == vec3(0, 0, 0))
    {
        FragColor = vec4(0, 1, 0, 1);
    }
    else
    {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);

        //specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * material.specular); ;
        //ambient
        vec3 ambient = light.ambient * material.ambient;
        //diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * material.diffuse);

        vec3 result = (ambient + diffuse + specular);
        FragColor = vec4(result.x, result.y, result.z, 1);
    }

};