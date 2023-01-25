#define BLUE   0.000f, 0.415f, 0.654f
#define YELLOW 0.996f, 0.800f, 0.008f

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    uv.x *= 16.0f; 
    uv.y *= 10.0f; 
    
    if (5.0 < uv.x && uv.x < 7.0) 
        fragColor = vec4(YELLOW, 1.0); 
    else if (4.0 < uv.y && uv.y < 6.0)
        fragColor = vec4(YELLOW, 1.0);
    else 
        fragColor = vec4(BLUE, 1.0);
}