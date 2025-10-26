#version 330 core

out vec4 FragColor;

in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_fragPos;

#define MAX_POINTLIGTS 16
#define MAX_SPOTLIGTS 16


struct DirLight {    
    vec3 position;  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {   
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
    float cutOff;     
    float outerCutOff;
};


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform DirLight dirLight; //TODO: maybe change to be the first element in the pointLight array
uniform PointLight pointLights[MAX_POINTLIGTS];
uniform SpotLight spotLights[MAX_SPOTLIGTS];

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