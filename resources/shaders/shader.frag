#version 330 core

out vec4 FragColor;

in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_fragPos;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform Light light;

vec3 phong(vec3 normal, vec3 viewDir, vec3 lightDir) {
    vec3 diffuseColor = light.diffuse * pow(texture(texture_diffuse1, v_texCoords).rgb, vec3(2.2)); // linearize color space
    vec3 ambientColor = light.ambient * diffuseColor;
    vec3 specularColor = light.specular * vec3(1.0);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;

    vec3 refl = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(viewDir, refl), 0.0), light.shininess);
    vec3 specular = spec * specularColor * texture(texture_specular1, v_texCoords).rgb;

    vec3 color = ambientColor + diffuse + specular;
    color = color / (color + vec3(1.0)); // Tone mapping
    color = pow(color, vec3(1.0/2.2)); // Gamma correction
    return color;
}

void main()
{   
    vec3 color = phong(normalize(v_normal), normalize(u_viewPos - v_fragPos), normalize(u_lightPos - v_fragPos));
    FragColor = vec4(color, 1.0);
}