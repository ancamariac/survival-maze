#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 3) in vec3 v_normal;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;
uniform float distortionScale;

out vec3 frag_color;
out vec3 frag_normal;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	float temp = rand(v_position.xy);

	frag_color = vec3(0.4f + distortionScale, 0.1f - distortionScale / 5.0f, 0.5f + temp/5.0f); 
	frag_normal = v_normal;

	gl_Position = Projection * View * Model * vec4(v_position + distortionScale * v_position * temp, 1.0);
}