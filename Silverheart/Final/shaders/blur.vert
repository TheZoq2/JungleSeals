attribute vec4 position;
attribute vec4 color;
attribute vec2 uv;
varying vec2 uvVarying;
varying vec4 colorVarying;
varying vec2 posVarying;
uniform mat4 agk_Ortho;
void main() { 
	gl_Position = agk_Ortho * position;
	posVarying = position.xy;
	uvVarying = uv;
	colorVarying = color;
}