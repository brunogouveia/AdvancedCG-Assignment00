//  NDC to RGB  vertex shader

/**
 *    We can do this just using the vertex shader. 
 *    We just need to set the right color for eache vertex, and all the other points will be an 
 *  interpolation (which is exactly what we wand).
 */

void main()
{
    //  Set vertex color
    gl_FrontColor = vec4((gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex) / gl_Position.w) * 0.5 + 0.5;
}
