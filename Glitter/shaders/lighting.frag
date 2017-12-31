#version 420 core
in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D emission;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
struct SpotLight {    
    vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};  


uniform sampler2D texture_diffuse;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular;
uniform sampler2D texture_specular2;
uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform vec3 viewPos;

uniform samplerCube skybox;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcEmission();

void main()
{
	// properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // phase 3: Spot light
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir); 
    result += CalcEmission();
	
	//reflection
	vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);

	//lighting
    //FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
    //ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

	float theta     = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);  
	diffuse  *= intensity;
	specular *= intensity;

	return (ambient + diffuse + specular);
}

vec3 CalcEmission() {
	/*Emission */
    vec3 emission = vec3(0.0);
    if (texture(material.texture_specular, TexCoords).r == 0.0)   /*rough check for blackbox inside spec texture */
    {
        /*apply emission texture */
        emission = texture(material.emission, TexCoords).rgb;
        
        /*some extra fun stuff with "time uniform" */
        //emission = texture(material.emission, TexCoords + vec2(0.0,time)).rgb;   /*moving */
        //emission = emission * (sin(time) * 0.5 + 0.5) * 2.0;                     /*fading */
    }

	return emission;
}