varying vec2 vTexCoord;
uniform float scale;

void main(void)
{
   vec4 model;

   model = gl_Vertex;

   model.x = model.x*scale;
   model.y = model.y*scale;
   model.z = model.z*scale;

   vTexCoord = vec2(gl_MultiTexCoord0);
   
   gl_Position = gl_ModelViewProjectionMatrix *model;

}