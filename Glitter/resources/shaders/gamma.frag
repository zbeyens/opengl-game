#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

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

vec3 BlinnPhong(vec3 lightPos, vec3 diffuseColor, vec3 specularColor);


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
	vec3 lighting = BlinnPhong(light.position, light.diffuse, light.specular);
	
	vec3 lightDir = normalize(light.position - fragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	lighting *= intensity;

	return (light.ambient + lighting);
}


vec3 BlinnPhong(vec3 lightPos, vec3 diffuseColor, vec3 specularColor)
{
	// diffuse
	vec3 lightDir = normalize(lightPos - fragPos);
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
	vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
	normal = normalize(fs_in.Normal);
	fragPos = fs_in.FragPos;
	viewDir = normalize(viewPos - fragPos);


	vec3 lighting = vec3(0.0);
	
	lighting += CalcDirLight(dirLight);

	//for (int i = 0; i < 4; ++i)
	//	lighting += BlinnPhong(lightPositions[i], lightColors[i], lightColors[i]);

	lighting += CalcSpotLight(spotLight); 

	// phase 2: Point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
		lighting += BlinnPhong(pointLights[i].position, pointLights[i].diffuse, pointLights[i].specular);

	color *= lighting;


	if (gamma)
		color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);


	//BlinnPhong(fs_in.Normal, fs_in.FragPos, )
	
	//// ambient
	//vec3 ambient = 0.05 * color;

	////vec3 specular = vec3(0.3) * spec; // assuming bright white light color
	//FragColor = vec4(ambient + diffuse + specular, 1.0);
}