#include "_light.h"

_light::_light(GLenum light)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(light);

    glLightfv(light, GL_AMBIENT, light_ambient);
    glLightfv(light, GL_DIFFUSE, light_diffuse);
    glLightfv(light, GL_SPECULAR,light_specular);
    glLightfv(light, GL_POSITION,light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS,high_shininess);

}

_light::~_light()
{
    //dtor
}
