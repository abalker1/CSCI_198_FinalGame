uniform sampler2D myTexture;

varying vec2 vTexCoord;

void main(void)
{
  gl_FragColor= texture2D(myTexture,vTexCoord).ggba;  
// changed colors of the texture

}