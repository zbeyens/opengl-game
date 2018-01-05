#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	//vec3 Normal;
	vec2 TexCoords;

	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
} fs_in;

in vec3 TangentLights[6];

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

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;

uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform sampler2D floorTexture;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 viewPos;
uniform bool gamma;
uniform bool blinn;

vec3 normal;
vec3 fragPos;
vec3 viewDir;

vec3 BlinnPhong(vec3 lightPos, vec3 diffuseColor, vec3 specularColor, vec3 tangentLight);


vec3 CalcDirLight(DirLight light)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	float spec = 0.0;
	if (blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}
	// combine results
	vec3 ambient  = light.ambient;
	vec3 diffuse  = light.diffuse  * diff;
	vec3 specular = light.specular * spec;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light)
{
	vec3 lighting = BlinnPhong(light.position, light.diffuse, light.specular, TangentLights[1]);
	
	vec3 lightDir = normalize(TangentLights[1] - fragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	lighting *= intensity;

	return (light.ambient + lighting);
}


vec3 BlinnPhong(vec3 lightPos, vec3 diffuseColor, vec3 specularColor, vec3 tangentLight)
{
	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
	//normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * diffuseColor;

	// specular
	float spec = 0.0;
	// specular
	if (blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
	}
	vec3 specular = spec * specularColor;

	// simple attenuation
	float max_distance = 1.5;
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (gamma ? distance * distance : distance);
	
	diffuse *= attenuation;
	specular *= attenuation;
	
	return diffuse + specular;
}

void main()
{
	 // obtain normal from normal map in range [0,1]
	normal = texture(normalMap, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	fragPos = fs_in.FragPos;
	viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	//viewDir = normalize(viewPos - fragPos);

	
	// get diffuse color
	vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;

	// ambient
	//vec3 ambient = 0.1 * color;
	// diffuse
	//vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	//float diff = max(dot(lightDir, normal), 0.0);
	//vec3 diffuse = diff * color;
	// specular
	//vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	//vec3 reflectDir = reflect(-lightDir, normal);
	//vec3 halfwayDir = normalize(lightDir + viewDir);
	//float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	//vec3 specular = vec3(0.2) * spec;
	//FragColor = vec4(ambient + diffuse + specular, 1.0);


	//vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
	//normal = normalize(fs_in.Normal);

	vec3 lighting = vec3(0.0);
	
	//lighting += CalcDirLight(dirLight);
	

	lighting += CalcSpotLight(spotLight);

	//// phase 2: Point lights
	//for (int i = 0; i < NR_POINT_LIGHTS; i++)
	//	lighting += BlinnPhong(pointLights[i].position, pointLights[i].diffuse, pointLights[i].specular, TangentLights[i + 2]);

	color *= lighting;


	if (gamma)
		color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);

   
}