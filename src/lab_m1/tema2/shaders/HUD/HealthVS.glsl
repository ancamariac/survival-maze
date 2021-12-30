#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 3) in vec3 v_normal;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

out vec3 frag_color;
out vec3 frag_normal;

void main()
{

	frag_color = vec3(0.6f, 0, 0); // 
	frag_normal = v_normal;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}