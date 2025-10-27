#version 330 core

out vec4 FragColor;

in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_fragPos;

#define MAX_POINTLIGTS 16
#define MAX_SPOTLIGTS 8


struct DirLight {    
    vec3 direction;  
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

struct Spotlight {   
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
uniform float shininess;
uniform DirLight dirLight; //TODO: maybe change to be the first element in the pointLight array
uniform PointLight pointLights[MAX_POINTLIGTS];
uniform Spotlight spotlights[MAX_SPOTLIGTS];
uniform ivec3 lightCount;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{   
    //vec3 color = phong(normalize(v_normal), normalize(u_viewPos - v_fragPos), normalize(u_lightPos - v_fragPos));
    vec3 viewDir = normalize(u_viewPos - v_fragPos);
    vec3 fragPos = v_fragPos;
    vec3 normal = normalize(v_normal);

    //Directional light
    vec3 color = lightCount[0] * calcDirLight(dirLight, normal, viewDir); //messy, change later
    //Point lights
    for(int i = 0; i < lightCount[1]; i++) {
        color += calcPointLight(pointLights[i], normal, fragPos, viewDir);
    }
    //Spot lights
    for(int i = 0; i < lightCount[2]; i++) {
        color += calcSpotlight(spotlights[i], normal, fragPos, viewDir);
    }

    FragColor = vec4(color, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, v_texCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, v_texCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, v_texCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, v_texCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, v_texCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, v_texCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 calcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);  
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, v_texCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, v_texCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, v_texCoords));
    ambient  *= intensity;
    diffuse  *= intensity;
    specular *= intensity;
    return (ambient + diffuse + specular);
}