#version 330
precision mediump float;

out vec4 FragColor;

uniform vec2 uResolution;
uniform float uTime;

float calc_saw( float y, float interval, float scale )
{
	return max( ( abs( interval / 2.0 - mod( y, interval ) ) - ( interval / 2.0 - 0.5 ) ) * scale, 0.0 );
}

void main( )
{
 float saw_b_shift = cos(uTime);
 float saw_a_interval = 1.0;
 float saw_b_interval = 2.0;
 float saw_a_scale = 1 - cos(uTime);
 float saw_b_scale = 0.821;
 float right = 1-cos(uTime / 2);
 float left = -1.0;

	vec4 color = vec4(0.352, 0, 0.4862, 1);
	vec2 scaled_uv = gl_FragCoord.xy / uResolution;

	float saw_a = calc_saw( scaled_uv.x, saw_a_interval, saw_a_scale );
	float saw_b = calc_saw( scaled_uv.x + saw_b_shift, saw_b_interval, saw_b_scale );


	FragColor = vec4(
		color.rgb
	,	color.a
		* float( scaled_uv.y < max( saw_a, saw_b ) + right )
		* float( max( saw_a, saw_b ) + left < scaled_uv.x )
	);
}