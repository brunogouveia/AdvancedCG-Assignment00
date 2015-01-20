//  NDC to RGB fragment shader

uniform float xSize;
uniform float ySize;

/**
 *   We can do this just using the FragCoord. 
 *   X and Y of gl_FragCoord, are the pixel coordinates in the window, so if we dived both by 
 * xSize (width of the window) and ySize(height of the window), we get the x and y values in the NDC. 
 * Z is already the right value, because it's used in the depth test.
 */

void main()
{
    gl_FragColor =  vec4(gl_FragCoord.x / xSize, gl_FragCoord.y / ySize, gl_FragCoord.z,  1);
}
