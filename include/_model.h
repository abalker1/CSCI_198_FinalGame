#ifndef _MODEL_H
#define _MODEL_H

#include<_common.h>
#include<_texture.h>

/* Vector */
typedef float vec3_t[3];

/* MD2 header */
struct md2_header_t
{
  int ident;
  int version;

  int skinwidth;
  int skinheight;

  int framesize;

  int num_skins;
  int num_vertices;
  int num_st;
  int num_tris;
  int num_glcmds;
  int num_frames;

  int offset_skins;
  int offset_st;
  int offset_tris;
  int offset_frames;
  int offset_glcmds;
  int offset_end;
};

/* Texture name */
struct md2_skin_t
{
  char name[64];
};

/* Texture coords */
struct md2_texCoord_t
{
  short s;
  short t;
};

/* Triangle info */
struct md2_triangle_t
{
  unsigned short vertex[3];
  unsigned short st[3];
};

/* Compressed vertex */
struct md2_vertex_t
{
  unsigned char v[3];
  unsigned char normalIndex;
};

/* Model frame */
struct md2_frame_t
{
  vec3_t scale;
  vec3_t translate;
  char name[16];
  struct md2_vertex_t *verts;
};

/* GL command packet */
struct md2_glcmd_t
{
  float s;
  float t;
  int index;
};

/* MD2 model structure */
struct md2_model_t
{
  struct md2_header_t header;

  struct md2_skin_t *skins;
  struct md2_texCoord_t *texcoords;
  struct md2_triangle_t *triangles;
  struct md2_frame_t *frames;
  int *glcmds;

  GLuint tex_id;
};

/* Table of precalculated normals */

class _model
{
    public:
        _model(float x, float y, float z);
        virtual ~_model();

        GLfloat s, t;
        int i, *pglcmds;
        vec3_t anorms_table[162] = {
        #include "Anorms.h"
        };

        _texture *texture = new _texture();
        vec3_t v_curr, v_next, v, norm;
        float *n_curr, *n_next;
        struct md2_frame_t *pframe, *pframe1, *pframe2;
        struct md2_vertex_t *pvert, *pvert1, *pvert2;
        struct md2_glcmd_t *packet;
        struct md2_model_t md2file;

        int readMD2Model (const char* filename, struct md2_model_t* mdl, const char* skinFile);
        void renderFrame (int n, const struct md2_model_t *mdl);
        virtual void renderFrameItpWithGLCmds (int n, float interp, const struct md2_model_t *mdl);
        virtual void animate (int start, int end, int &frame, float &interp);
        void initModel(const char* modelFile, const char* skinFile);
        void draw(double dt, bool paused);
        void freeModel (struct md2_model_t *mdl);
        void actions();

        enum Action {
            ACT_STAND,
            ACT_RUN,
            ACT_ATTACK,
            ACT_PAIN_A,
            ACT_PAIN_B,
            ACT_PAIN_C,
            ACT_JUMP,
            ACT_FLIP,
            ACT_SALUTE,
            ACT_TAUNT,
            ACT_WAVE,
            ACT_POINT,
            ACT_CROUCH_IDLE,
            ACT_CROUCH_WALK,
            ACT_CROUCH_ATTACK,
            ACT_CROUCH_PAIN,
            ACT_CROUCH_DEATH,
            ACT_DEATH_BACK,
            ACT_DEATH_FORWARD,
            ACT_DEATH_GIB,
        };
        int actionTrigger =0;  //Action Trigger
        int startFrame =0;
        int endFrame;
        int n;
        float interp;
        bool paused;

        vec3 pos;
        float dirAngleZ=180;

    protected:

    private:
};

#endif // _MODEL_H
