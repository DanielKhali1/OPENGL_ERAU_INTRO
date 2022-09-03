#version 430

uniform mat4 translationMatrix;
uniform mat4 rotateMatrix;
uniform mat4 rotation2Matrix;

out vec4 color;
void main()
{
	if (gl_VertexID == 0) {
		gl_Position = translationMatrix * rotateMatrix * rotation2Matrix*  vec4( 0.25, -0.25, 0.0, 1.0);
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if (gl_VertexID == 1) {
		gl_Position = translationMatrix* rotateMatrix * rotation2Matrix*vec4(-0.25, -0.25, 0.0, 1.0);
		color = vec4(0.0, 1.0, 0.0, 1.0);
	}
	else {
		gl_Position = translationMatrix * rotateMatrix *rotation2Matrix* vec4( 0.0, 0.5, 0.0, 1.0);
		color = vec4(0.0, 0.0, 1.0, 1.0);
	}
}