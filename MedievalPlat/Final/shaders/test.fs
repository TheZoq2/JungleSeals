float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float hash( float n ) //Borrowed from voltage
{
    return fract(sin(n)*43758.5453);
}

float fBmWRand( vec2 p )//Borroowed from Mark Sleith
{
    	float f = 0.0;
    	f += 0.50000*rand( p ); p = p*2.02;
    	f += 0.25000*rand( p ); p = p*2.03;
    	f += 0.12500*rand( p ); p = p*2.01;
    	f += 0.06250*rand( p ); p = p*2.04;
    	f += 0.03125*rand( p );
    	return f/0.984375;
}

float noise( in vec2 x )//Borroowed from Mark Sleith
{
	vec2 p = floor(x);
	vec2 f = fract(x);
    	f = f*f*(3.0-2.0*f);
    	float n = p.x + p.y*57.0;
    	float res = mix(mix( hash(n+  0.0), hash(n+  1.0),f.x), mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
    	return res;
}

void main()
{
	vec4 finalColor = vec4(bgGradient(), 1.0);
	
	gl_FragColor = vec4(finalColor);
}