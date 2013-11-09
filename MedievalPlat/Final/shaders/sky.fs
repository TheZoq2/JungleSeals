uniform vec2 iResolution;
uniform vec2 iMouse;

uniform float time;

uniform int cloudLayers;
uniform float overcast;
uniform float posX;
uniform float posY;

uniform vec4 dSky; //The color of the sky during the day
uniform vec4 nSky; //The color of the sky during the night

uniform vec4 dCloud; //The color of the clouds at day
uniform vec4 nCloud; //The color of the clouds at night

float resFact = iResolution.x / 500;

//float overcast = iMouse.y / iResolution.y;
//float posX = iMouse.x / iResolution.x;

/*float nSkyR = 0.05;
float nSkyG = 0.05;
float nSkyB = 0.39;*/

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

float fbm( vec2 p ) //Borroowed from Mark Sleith
{
    	float f = 0.0;
    	f += 0.50000*noise( p ); p = p*2.02;
    	f += 0.25000*noise( p ); p = p*2.03;
    	f += 0.12500*noise( p ); p = p*2.01;
    	f += 0.06250*noise( p ); p = p*2.04;
    	f += 0.02125*noise( p );
    	return f/0.984375;
}

vec3 bgGradient()
{
	//Getting the height of the current pixel
	float height = gl_FragCoord.y / iResolution.y;
	
	//Calculating the brightness of the pixel
	float brightness = 1.0 - 0.4 * height;
	
	//Combining everything into a background
	vec3 grad = vec3(1., 1., 1.);// * brightness;
	return grad;
}

bool star()
{
	/*//Getting a position to run random calculations with
	float pos = (gl_FragCoord.x / iResolution.x) * (gl_FragCoord.y / iResolution.y) + 0.5;
	
	if(hash(pos) < 0.001)
	{
		return true;
	}
	return false*/;
		
	if(fBmWRand(gl_FragCoord.xy / iResolution.xy) < 0.08)
	{
		return true;
	}
	return false;
}

float cloudFadeDist = 0.2; //The distance at which the clouds will start fading away

vec4 clouds( vec2 point )
{
	vec4 result = vec4(0., 0., 0., 0.);
	
	//Checking if the cloud is above
	float fbmResult = fbm(point * 5.);
	if(fbmResult > overcast)
	{
		result = vec4(fbmResult, fbmResult, fbmResult, 1.0);
		//result = vec4(1., 1., 1., 1.);
	}
	else if(fbmResult > overcast - (cloudFadeDist / resFact)) //Outlining the clouds
	{
		float dist = overcast - fbmResult;
		float colorFac = 1. - dist / (cloudFadeDist / resFact);
		
		//if(colorFac > cloudFadeDist)
		//{
			result = vec4(fbmResult, fbmResult, fbmResult, colorFac);
		//}
	}
	
	//Finer details
	float fbmDetail = fbm(point * 20.);
	vec4 details = vec4( .5 + fbmDetail, .5 + fbmDetail, .5 + fbmDetail, 1.0);
	
	//result = mix(result, details, result.a);
	result = vec4(details.r, details.g, details.b, result.a);
	
	
	float oldAlpha = result.a; //Saving the old alpha
	//Making the clouds brighter
	result = result * .5;
	result = result + .45;
	//Saving the new alpha
	result.a = oldAlpha;
	
	return result;
}

vec2 sunPos = vec2(0.15, 0.1);

float sunWidth = 0.03;
float sunGlow = 0.015;

float sunR = 1.;
float sunG = 1.;
float sunB = 0.8;

vec4 sun()
{
	vec4 result = vec4(0., 0., 0., 0.);
	float xPos = gl_FragCoord.x / iResolution.x;
	float yPos = gl_FragCoord.y / iResolution.x;
	
	float xDist = xPos - sunPos.x;
	float yDist = yPos - sunPos.y;
	
	float dist = sqrt(pow(xDist, 2.) + pow(yDist, 2.));
	
	if(dist < sunWidth)
	{
		result = vec4(sunR, sunG, sunB, 1.);
	}
	else if(dist < sunWidth + sunGlow)
	{
		float distFact = (dist - sunWidth) / sunGlow;
		
		result = vec4(sunR, sunG, sunB , 1. - distFact);
	}
	
	return result;
}

float nStart = 2200;
float nEnd = 600;
float dStart = 800;
float dEnd = 2000;

void main(void)
{
	//Cretaing the final color variable and adding the gradient
	vec4 finalColor = vec4(bgGradient(), 1.0);
	
	//Creating stars
	vec4 starLayer = vec4(0., 0., 0., 0.);
	if(star() == true)
	{
		starLayer = vec4(1., 1., 1., 1.);	//Make the pixel very bright
	}

	//Generating the clouds
	vec4 cloudLayer = vec4(0., 0., 0., 0.);
	//for(int i = 0; i < tempCloudLayers; i++)
	
	vec4 cloudColor = vec4(0., 0., 0., 0.);

	if(time > nStart || time < nEnd) //Nighttime
	{
		finalColor = finalColor * nSky;
		
		finalColor = mix(finalColor, starLayer, starLayer.a);
		
		//cloudLayer = cloudLayer * nCloud;
		cloudColor = nCloud;
		//finalColor = mix(finalColor, cloudLayer, cloudLayer.a);
		//vec4 genericColor = cloudLayer;
		//finalColor = mix(finalColor, cloudLayer, cloudLayer.a);
		//finalColor = vec4(nSky.r, nSky.g, nSky.b, 1.0);
	}
	else if(time > dStart && time < dEnd)
	{
		finalColor = finalColor * dSky;
		
		cloudColor = dCloud;
		//cloudLayer = cloudLayer * dCloud;
		//finalColor = mix(finalColor, cloudLayer, cloudLayer.a);
	}
	else if(time > dEnd && time < nStart) //Evening
	{
		float timeFact = (time - dEnd) / (nStart - dEnd);
		
		//Calculating the diffirence between night and day
		vec4 skyDiff = vec4(nSky.r - dSky.r, nSky.g - dSky.g, nSky.b - dSky.b, 1.);
		vec4 skyColor = vec4(dSky.r + (skyDiff.r * timeFact), dSky.g + (skyDiff.g * timeFact), dSky.b + (skyDiff.b * timeFact), 1.);
		
		finalColor = finalColor * skyColor;
		
		//Stars
		finalColor = mix(finalColor, starLayer, starLayer * timeFact);
		
		//Clouds
		vec4 cloudDiff = vec4(nCloud.r - dCloud.r, nCloud.g - dCloud.g, nCloud.b - dCloud.b, 1.);
		cloudColor = vec4(dCloud.r + (cloudDiff.r * timeFact), dCloud.g + (cloudDiff.g * timeFact), dCloud.b + (cloudDiff.b * timeFact), 1.);
		//vec4 cloudLayer = cloudLayer * cloudColor;
		//finalColor = mix(finalColor, cloudLayer, cloudLayer.a);
	}
	else if(time > nEnd && time < dStart) //Morning
	{
		float timeFact = (time - nEnd) / (dStart - nEnd);
		
		//Calculating the diffirence between night and day
		vec4 skyDiff = vec4(dSky.r - nSky.r, dSky.g - nSky.g, dSky.b - nSky.b, 1.);
		vec4 skyColor = vec4(nSky.r + (skyDiff.r * timeFact), nSky.g + (skyDiff.g * timeFact), nSky.b + (skyDiff.b * timeFact), 1.);
		//Calculating the diffirence between night and day

		finalColor = finalColor * skyColor;
		
		//Stars
		finalColor = mix(finalColor, starLayer, starLayer * (1. - timeFact));
		
		//Clouds
		vec4 cloudDiff = vec4(dCloud.r - nCloud.r, dCloud.g - nCloud.g, dCloud.b - nCloud.b, 1.);
		cloudColor = vec4(nCloud.r + (cloudDiff.r * timeFact), nCloud.g + (cloudDiff.g * timeFact), nCloud.b + (cloudDiff.b * timeFact), 1.);
		//vec4 cloudLayer = cloudLayer * cloudColor;
		//finalColor = mix(finalColor, cloudLayer, cloudLayer.a);
	}
	
	//finalColor = vec4(1., 0., 0., 1.);
	
	//vec4 sunColor = sun();
	//finalColor = mix(finalColor, sunColor, sunColor.a);
	//If there is a star
	/*if(star() == true)
	{
		finalColor = vec4(1., 1., 1., 1.);	//Make the pixel very bright
	}*/
	
	/*for(int i = 0; i < 4; i++)
	{
		//clouds( (15.0 * float(i)) + gl_FragCoord.xy / iResolution.xy + posX * float(i + 1));
		//vec4 cloud = clouds( vec2((15. * float(i)) + gl_FragCoord.x / iResolution.x + posX * float(i + 1)),
							(15. * float(i)) + gl_FragCoord.y / iResoulution.y);
		
		vec4 cloud = clouds( vec2( (15. * float(i)) + gl_FragCoord.x / iResolution.x + posX * float(i + 1),
			(15. * float(i)) + gl_FragCoord.y / iResolution.y + posY * float(i + 1)));
		
		//finalColor = finalColor + vec4(cloud, 1.);
		finalColor = mix(finalColor, cloud, cloud.a);
	}*/
	
	for(int i = 2; i >= 0; i--)
	{
		//clouds( (15.0 * float(i)) + gl_FragCoord.xy / iResolution.xy + posX * float(i + 1));
		/*vec4 cloud = clouds( vec2((15. * float(i)) + gl_FragCoord.x / iResolution.x + posX * float(i + 1)),
							(15. * float(i)) + gl_FragCoord.y / iResoulution.y);*/
		
		vec4 cloud = clouds( vec2( (15. * float(i)) + gl_FragCoord.x / iResolution.x + posX * float(i + 1),
			(15. * float(i)) + gl_FragCoord.y / iResolution.y + posY * float(i + 1)));
		
		//finalColor = finalColor + vec4(cloud, 1.);
		finalColor = mix(finalColor, cloud * cloudColor, cloud.a);
		
		//cloudLayer = vec4(1., 1., 1., 1.);
	}
	
	gl_FragColor = finalColor;
}