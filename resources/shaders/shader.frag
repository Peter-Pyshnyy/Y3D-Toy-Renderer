#version 330 core

out vec4 FragColor;

in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_fragPos;

#define MAX_POINTLIGHTS 16
#define MAX_SPOTLIGHTS 8

struct LightComps {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {    
    LightComps multipliers;
    vec3 direction;  
};

struct PointLight {
    LightComps multipliers;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct Spotlight {   
    LightComps multipliers;
    vec3 position;
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
uniform PointLight pointLights[MAX_POINTLIGHTS];
uniform Spotlight spotlights[MAX_SPOTLIGHTS];
uniform uvec3 lightCount;

vec3 phong(LightComps multipliers, vec3 normal, vec3 viewDir, vec3 lightDir);
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
    for(uint i = 0u; i < lightCount[1]; i++) {
        color += calcPointLight(pointLights[i], normal, fragPos, viewDir);
    }
//    //Spot lights
    for(uint i = 0u; i < lightCount[2]; i++) {
        color += calcSpotlight(spotlights[i], normal, fragPos, viewDir);
    }

    FragColor = vec4(color, 1.0);
}

vec3 phong(LightComps multipliers, vec3 normal, vec3 viewDir, vec3 lightDir) {
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 ambient  = multipliers.ambient  * vec3(texture(texture_diffuse1, v_texCoords));
    vec3 diffuse  = multipliers.diffuse  * diff * vec3(texture(texture_diffuse1, v_texCoords));
    vec3 specular = multipliers.specular * spec * vec3(texture(texture_specular1, v_texCoords));
    return ambient + diffuse + specular;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    return phong(light.multipliers, normal, viewDir, lightDir);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    return phong(light.multipliers, normal, viewDir, lightDir) * attenuation;
} 

vec3 calcSpotlight(Spotlight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    return phong(light.multipliers, normal, viewDir, lightDir) * intensity;
}