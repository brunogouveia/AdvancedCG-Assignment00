//  Basic vertex shader

varying vec4 v_Pos;

/**
 *   We can do this in two steps:
 * First: we pass the normalized vertex position through v_Pos, which is a point inside NDC.
 * Second: we get the interpolated normalized vertex inside the fragment shader, and map it to the correspondent RGB Color.
 */

void main()
{
    //  Set vertex coordinates
    gl_Position = v_Pos = gl_ModelViewProjectionMatrix * gl_Vertex;
}
