#version 450 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 fragWorldPos;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float specularStrength;
uniform float ambientStrength;

// texture
uniform sampler2D Texture;

out vec4 color;

void main(){
		
		//color = texture(Texture, TexCoord);
		
		vec3 norm = normalize(Normal);
		vec4 objColor = texture(Texture, TexCoord);

		//**ambient
		vec3 ambient = ambientStrength * lightColor;
		
		//**diffuse
		vec3 lightDir = normalize(lightPos - fragWorldPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;
		
		//**specular 
		vec3 viewDir = normalize(cameraPos - fragWorldPos);
		vec3 reflectionDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectionDir),0.0),128);
		vec3 specular = specularStrength * spec * lightColor;
		
		// lighting calculation
		

		vec3 totalColor = (ambient + diffuse) * objColor.rgb;

		color = vec4(totalColor, 1.0f);
		
}