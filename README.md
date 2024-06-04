# Computer Graphics II - Homework III
<img src="final_output_1.png" alt="final_output_1" width="250"/> <img src="final_output_2.png" alt="final_output_2" width="250"/> 
## Grass Blade Generation
Generating grass blades from vertices is straightforward with the geometry shader stage. We can just render a simple triangle from the input vertex, a little offset vertex, and a vertex that is at the top of the input vertex with a height of grass blade height we choose. For example, we can do something like this in geometry shader (simplified):

```glsl

#version 460 core
for(int i = 0; i < 3; i++)
{
  vec3 position = fragWorldPos[i];
  vec3 normal = normalize(fragWorldNor[i]);
  vec3 offset = vec3(-0.01, 0.00, 0.00) * pow(2, (bladeHeight * 2));
  vec4 point1 = vec4(position, 1.0);
  gl_Position = projection * view * point1;
  EmitVertex();
  vec4 point2 = vec4(position + offset, 1.0);
  gl_Position = projection * view * point2;
  EmitVertex();
  vec4 point3 = vec4(position + normal * bladeHeight, 1.0);
  gl_Position = projection * view * point3;
  EmitVertex();
  EndPrimitive();
}
```
<img src="straight_grass.png" alt="Straight Grass blades" width="700"/> 
However, as you can see, this would generate very sharp, knife-like grass blades. As we know, grass blades in real life have a curve-like structure. So, we will use bezier curves to generate grass blades to achieve that. 

Bezier curves are defined by 4 control points, and there is no convention, i.e., we must determine the values with hands so that the resulting grass blades will look real, try and fail approach. My approach is very simple and can be seen in the picture below: 

<img src="bezier_implement.png" alt="bezier_implement" width="700"/> 

Blade height will be adjustable as the task demands so no need to find a fine value for it, but bezierOffset is not. I determined a basic little offset value and scaled it by the blade height, so the resulting blades would not be too thin when they are too long. 

```glsl
for (int i = 0; i < 3; ++i)
    {
        vec3 position = fragWorldPos[i];
        vec3 normal = normalize(fragWorldNor[i]);
        vec3 bezierOffset = vec3(-0.01, 0.00, 0.00) * pow(2, (bladeHeight * 2));
      
        vec4 p1 = vec4(position, 1.0);
        vec4 p2 = vec4(position + bezierOffset + normal * bladeHeight * 0.333, 1.0);
        vec4 p3 = vec4(position + bezierOffset + normal * bladeHeight * 0.667, 1.0);
        vec4 p4 = vec4(position + normal * bladeHeight, 1.0);
               
        for (int j = 0; j <= 8; ++j)
        {
            vec4 pos = bezier(p1, p2, p3, p4, tVal);
            t = tVal;
            gl_Position = projection * view * pos;
            EmitVertex();

            if (j < 8)
            {
                gl_Position = projection * view * (pos + vec4(0.02 * (1.0 - tVal), 0.0, 0.0, 0.0));
            }
            else
            {
                gl_Position = projection * view * p4;
            }
            EmitVertex();
        }
        EndPrimitive();
    }
```
In the above code piece, tval is used for how far away the fragment is from the root of the grass. We use this tval to shade the grass blades. To be more precise, if the fragments are close to the bottom of the grass, they will be darker,  so the overall look will be like there are shadows in the lower altitude even though there is no implementation for shadows, just a perception trick. Also, the tval is used to determine the fragment's positions. In the previous implementation, we used just 3 pieces, but as we are generating a curve, we must use more fragments to draw. As the fragment altitude gets higher, the offset that is used  will get smaller. So the resulting drawing structure will be like this:

<img src="polygon_grass.png" alt="polygon_grass.png" width="700"/> 

And, final grass drawing:

<img src="grass_no_wind.png" alt="grass_no_wind.png" width="700"/> 
