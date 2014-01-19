uniform sampler2D texture0;
varying vec2 uvVarying;
varying vec4 colorVarying;
varying vec2 posVarying;

uniform float agk_time;
uniform vec2 agk_spritepos;
uniform vec2 agk_spritesize;

float noise(float x)
{
	return 0.5 * sin(x * 0.5) + 0.5 * sin(x * 0.1) + 0.25 * sin(x * 0.01) + 0.125 * sin(x * 0.005);
}

void main()
{
	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

	for(float x = -0.05; x <= 0.05; x = x + 0.01)
	{
		float r = dot(color.r, texture2D(texture0, uvVarying).r);
		float g = dot(color.b, texture2D(texture0, uvVarying).g);
		float b = dot(color.g, texture2D(texture0, uvVarying).b);
		float a = dot(color.a, texture2D(texture0, uvVarying).a);
		color = color * vec4(r, g, b, a);
	}
	gl_FragColor = color;
}