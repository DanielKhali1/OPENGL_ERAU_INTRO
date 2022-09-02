#version 430

uniform float offset;
uniform mat4 rotateMatrix;

out vec4 color;
void main()
{
	if (gl_VertexID == 0) {
		gl_Position = vec4( 0.25, -0.25, 0.0, 1.0) ;
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if (gl_VertexID == 1) {
		gl_Position = vec4(-0.25, -0.25, 0.0, 1.0);
		color = vec4(0.0, 1.0, 0.0, 1.0);
	}
	else {
		gl_Position = vec4( 0, 0.5, 0.0, 1.0);
		color = vec4(0.0, 0.0, 1.0, 1.0);
	}
}