#include "_model.h"

// A lot of the code here has been reformatted because... Well, it needed to be.

_model::_model(float x, float y, float z)
{
    //ctor
    this->pos.x = x;
    this->pos.y = y;
    this->pos.z = z;

    n = 0; /* The current frame */
    interp = 0.0;
}

_model::~_model()
{
    freeModel(&md2file);
}

int _model::readMD2Model(const char* filename, struct md2_model_t* mdl, const char* skinFile)
{
     FILE *fp;

  fp = fopen (filename, "rb");
  if (!fp)
    {
      fprintf (stderr, "Error: couldn't open \"%s\"!\n", filename);
      return 0;
    }

  /* Read header */
  fread (&mdl->header, 1, sizeof (struct md2_header_t), fp);

  if ((mdl->header.ident != 844121161) ||
      (mdl->header.version != 8))
    {
      /* Error! */
      fprintf (stderr, "Error: bad version or identifier\n");
      fclose (fp);
      return 0;
    }

  /* Memory allocations */
  mdl->skins = (struct md2_skin_t *)malloc (sizeof (struct md2_skin_t) * mdl->header.num_skins);
  mdl->texcoords = (struct md2_texCoord_t *)malloc (sizeof (struct md2_texCoord_t) * mdl->header.num_st);
  mdl->triangles = (struct md2_triangle_t *)malloc (sizeof (struct md2_triangle_t) * mdl->header.num_tris);
  mdl->frames = (struct md2_frame_t *)malloc (sizeof (struct md2_frame_t) * mdl->header.num_frames);
  mdl->glcmds = (int *)malloc (sizeof (int) * mdl->header.num_glcmds);

  /* Read model data */
  fseek (fp, mdl->header.offset_skins, SEEK_SET);
  fread (mdl->skins, sizeof (struct md2_skin_t),mdl->header.num_skins, fp);

  fseek (fp, mdl->header.offset_st, SEEK_SET);
  fread (mdl->texcoords, sizeof (struct md2_texCoord_t),mdl->header.num_st, fp);

  fseek (fp, mdl->header.offset_tris, SEEK_SET);
  fread (mdl->triangles, sizeof (struct md2_triangle_t),mdl->header.num_tris, fp);

  fseek (fp, mdl->header.offset_glcmds, SEEK_SET);
  fread (mdl->glcmds, sizeof (int), mdl->header.num_glcmds, fp);

  /* Read frames */
  fseek (fp, mdl->header.offset_frames, SEEK_SET);
  for (int i = 0; i < mdl->header.num_frames; ++i)
    {
      /* Memory allocation for vertices of this frame */
      mdl->frames[i].verts = (struct md2_vertex_t *)malloc (sizeof (struct md2_vertex_t) * mdl->header.num_vertices);

      /* Read frame data */
      fread (mdl->frames[i].scale, sizeof (vec3_t), 1, fp);
      fread (mdl->frames[i].translate, sizeof (vec3_t), 1, fp);
      fread (mdl->frames[i].name, sizeof (char), 16, fp);
      fread (mdl->frames[i].verts, sizeof (struct md2_vertex_t),mdl->header.num_vertices, fp);

    }

    texture->load(skinFile);
    mdl->tex_id = texture->textureID;

    endFrame = mdl->header.num_frames-1;

  fclose (fp);
  return 1;
}

void _model::renderFrame(int n, const struct md2_model_t* mdl)
{
    /* Check if n is in a valid range */
    if ((n < 0) || (n > mdl->header.num_frames - 1))
        return;

    /* Enable model's texture */
    glBindTexture (GL_TEXTURE_2D, mdl->tex_id);

    /* Draw the model */
    glBegin (GL_TRIANGLES);

    /* Draw each triangle */
    for (int i = 0; i < mdl->header.num_tris; ++i)
    {
        /* Draw each vertex */
        for (int j = 0; j < 3; ++j)
        {
            pframe = &mdl->frames[n];
            pvert = &pframe->verts[mdl->triangles[i].vertex[j]];

            /* Compute texture coordinates */
            s = (GLfloat)mdl->texcoords[mdl->triangles[i].st[j]].s / mdl->header.skinwidth;
            t = (GLfloat)mdl->texcoords[mdl->triangles[i].st[j]].t / mdl->header.skinheight;

            /* Pass texture coordinates to OpenGL */
            glTexCoord2f (s, t);

            /* Normal vector */
            glNormal3fv (anorms_table[pvert->normalIndex]);

            /* Calculate vertex real position */
            v[0] = (pframe->scale[0] * pvert->v[0]) + pframe->translate[0];
            v[1] = (pframe->scale[1] * pvert->v[1]) + pframe->translate[1];
            v[2] = (pframe->scale[2] * pvert->v[2]) + pframe->translate[2];

            glVertex3fv (v);
        }
    }
    glEnd ();
}

void _model::renderFrameItpWithGLCmds(int n, float interp, const struct md2_model_t* mdl)
{
    /* Check if n is in a valid range */
    if ((n < 0) || (n > mdl->header.num_frames - 1))
        return;

    /* Enable model's texture */
    glBindTexture (GL_TEXTURE_2D, mdl->tex_id);

    /* pglcmds points at the start of the command list */
    pglcmds = mdl->glcmds;

    /* Draw the model */
    while ((i = *(pglcmds++)) != 0)
    {
        if (i < 0)
        {
            glBegin (GL_TRIANGLE_FAN);
            i = -i;
        }
        else
        {
            glBegin (GL_TRIANGLE_STRIP);
        }

        /* Draw each vertex of this group */
        for (/* Nothing */; i > 0; --i, pglcmds += 3)
        {
          packet = (struct md2_glcmd_t *)pglcmds;

          // Fix interpolation issues, add custom animation freezing for better aesthetics
            size_t next;
            next = (n < endFrame) ? n + 1 : startFrame;

            pframe1 = &mdl->frames[n];
            pframe2 = &mdl->frames[next];
            pvert1 = &pframe1->verts[packet->index];
            pvert2 = &pframe2->verts[packet->index];

          /* Pass texture coordinates to OpenGL */
          glTexCoord2f (packet->s, packet->t);

          /* Interpolate normals */
          n_curr = anorms_table[pvert1->normalIndex];
          n_next = anorms_table[pvert2->normalIndex];

          norm[0] = n_curr[0] + interp * (n_next[0] - n_curr[0]);
          norm[1] = n_curr[1] + interp * (n_next[1] - n_curr[1]);
          norm[2] = n_curr[2] + interp * (n_next[2] - n_curr[2]);

          glNormal3fv (norm);

          /* Interpolate vertices */
          v_curr[0] = pframe1->scale[0] * pvert1->v[0] + pframe1->translate[0];
          v_curr[1] = pframe1->scale[1] * pvert1->v[1] + pframe1->translate[1];
          v_curr[2] = pframe1->scale[2] * pvert1->v[2] + pframe1->translate[2];

          v_next[0] = pframe2->scale[0] * pvert2->v[0] + pframe2->translate[0];
          v_next[1] = pframe2->scale[1] * pvert2->v[1] + pframe2->translate[1];
          v_next[2] = pframe2->scale[2] * pvert2->v[2] + pframe2->translate[2];

          v[0] = v_curr[0] + interp * (v_next[0] - v_curr[0]);
          v[1] = v_curr[1] + interp * (v_next[1] - v_curr[1]);
          v[2] = v_curr[2] + interp * (v_next[2] - v_curr[2]);

          glVertex3fv (v);
        }
        glEnd ();
    }
}

void _model::animate(int start, int end, int& frame, float& interp)
{
    if ((frame < start) || (frame > end)){
        frame = start;
    }

    if (interp >= 1.0f)
    {
        interp = 0.0f;
        (frame)++;

        if (frame > end) {
            frame = start;
        }
    }
}

void _model::initModel(const char* modelFile, const char* skinFile)
{
     /* Load MD2 model file */
    if (!readMD2Model (modelFile, &md2file, skinFile))
    exit (EXIT_FAILURE);
}

void _model::draw(double dt, bool paused)
{
  /*Timing moved outside Draw function*/

  /* Animate model from frames 0 to num_frames-1 */

    glEnable(GL_TEXTURE_2D);
    if (!paused) {
        interp += 10 * (dt);
        animate (startFrame, endFrame, n, interp);
    }

    renderFrameItpWithGLCmds (n, interp, &md2file);
    glDisable(GL_TEXTURE_2D);
}

// As far as I can tell most of the quake models have these actions - Some have extras - Weapons don't have any death animations, so decouple any weapon animations before the player begins their death animation
void _model::actions()
{
    switch (actionTrigger)
    {
        case ACT_STAND:         startFrame =   0; endFrame =  39; break;
        case ACT_RUN:           startFrame =  40; endFrame =  45; break;
        case ACT_ATTACK:        startFrame =  46; endFrame =  53; break;
        case ACT_PAIN_A:        startFrame =  54; endFrame =  57; break;
        case ACT_PAIN_B:        startFrame =  58; endFrame =  61; break;
        case ACT_PAIN_C:        startFrame =  62; endFrame =  65; break;
        case ACT_JUMP:          startFrame =  66; endFrame =  71; break;
        case ACT_FLIP:          startFrame =  72; endFrame =  83; break;
        case ACT_SALUTE:        startFrame =  84; endFrame =  94; break;
        case ACT_TAUNT:         startFrame =  95; endFrame = 111; break;
        case ACT_WAVE:          startFrame = 112; endFrame = 122; break;
        case ACT_POINT:         startFrame = 123; endFrame = 134; break;
        case ACT_CROUCH_IDLE:   startFrame = 135; endFrame = 153; break;
        case ACT_CROUCH_WALK:   startFrame = 154; endFrame = 159; break;
        case ACT_CROUCH_ATTACK: startFrame = 160; endFrame = 168; break;
        case ACT_CROUCH_PAIN:   startFrame = 169; endFrame = 172; break;
        case ACT_CROUCH_DEATH:  startFrame = 173; endFrame = 177; break;
        case ACT_DEATH_BACK:    startFrame = 178; endFrame = 183; break;
        case ACT_DEATH_FORWARD: startFrame = 184; endFrame = 189; break;
        case ACT_DEATH_GIB:     startFrame = 190; endFrame = 197; break;
        default:                startFrame =   0; endFrame =  39; break;
    }
}

void _model::freeModel(struct md2_model_t* mdl)
{
    if (mdl->skins)
    {
        free (mdl->skins);
        mdl->skins = NULL;
    }

    if (mdl->texcoords)
    {
        free (mdl->texcoords);
        mdl->texcoords = NULL;
    }

    if (mdl->triangles)
    {
        free (mdl->triangles);
        mdl->triangles = NULL;
    }

    if (mdl->glcmds)
    {
        free (mdl->glcmds);
        mdl->glcmds = NULL;
    }

    if (mdl->frames)
    {

        for (int i = 0; i < mdl->header.num_frames; ++i)
        {
            free (mdl->frames[i].verts);
            mdl->frames[i].verts = NULL;
        }

        free (mdl->frames);
        mdl->frames = NULL;
    }
}


