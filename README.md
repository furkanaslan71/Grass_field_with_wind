# Computer Graphics II - Homework III
<!-- insert final images -->
## Grass Blade Generation
Generating grass blades from vertices is straightforward with the geometry shader stage. We can just render a simple triangle from the input vertex, a little offset vertex, and a vertex that is at the top of the input vertex with a height of grass blade height we choose. For example, we can do something likes this in geometry shader (simplified):

```glsl

#version 460 core
for(int i = 0; i < 3; i++)
{
  vec3 position = fragWorldPos[i];
  vec3 normal = normalize(fragWorldNor[i]);
  vec3 bezierOffset = vec3(-0.01, 0.00, 0.00) * pow(2, (bladeHeight * 2));
  vec4 point1 = vec4(position, 1.0);
  gl_Position = projection * view * point1;
  EmitVertex();
  vec4 point2 = vec4(position + bezierOffset, 1.0);
  gl_Position = projection * view * point2;
  EmitVertex();
  vec4 point3 = vec4(position + bezierOffset * normal * bladeHeight, 1.0);
  gl_Position = projection * view * point2;
  EmitVertex();
  EndPrimitive();
}

