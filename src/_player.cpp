#include "_player.h"

_player::_player(float x, float y, float z) : _model(x, y, z)
{
    this->grounded = false; // Player should start un-grounded so they can fall and find their position on the first platform they encounter
}

_player::~_player()
{
    //dtor
}


bool _player::jump()
{
    // If the player isn't near the ground, they can't jump
    if(!grounded) {
        return false;
    }

    playingOneShot = true;
    activeOneShot = ACT_JUMP;
    resumeAction = ACT_STAND;
    actionTrigger = ACT_JUMP;

    vy = jumpSpeed;
    grounded = false;
    return true;
}

void _player::step(float dt)
{
    // Return early if the player is on the ground;
    if (grounded) {
        pos.y = groundY;
        vy = 0.0f;
        return;
    }

    pos.y += vy *dt;
    float appliedGravity = gravity;
    appliedGravity *= (vy >0.0f) ? (jumpHeld ? gravityHeldMult : gravityCutMult) : gravityFallMult;

    vy = std::max(vy - appliedGravity * dt, -terminalVelocity);
}


void _player::playAnim(Action anim, Action resume)
{
    activeOneShot = anim;
    resumeAction = resume;
    playingOneShot = true;

    actionTrigger = anim;
    actions();
    n = startFrame;
    interp = 0.0f;
}

void _player::animate(int start, int end, int& frame, float& interp)
{
    if ((frame < start) || (frame > end)) {
        frame = start;
    }

    if (interp >= 1.0f) {
        interp = 0.0f;
        if (actionTrigger != ACT_JUMP) {
            ++(frame);
            if (frame > end) {
            if (playingOneShot && actionTrigger == activeOneShot) {
                    frame = end;
                    playingOneShot = false;
                    actionTrigger = resumeAction;
                    actions();
                    frame = startFrame;
                    return;
                }
                frame = start;
            }
        }
        else {
            int mid = (start + end) / 2;
            ++(frame);
            if (vy > 0 && frame > mid) {
                frame = mid;
            }
            if (vy <= 0 && frame > end) {
                frame = end;
                if (playingOneShot && actionTrigger == activeOneShot) {
                    playingOneShot = false;
                    actionTrigger = resumeAction;
                    actions();
                    frame = startFrame;
                    return;
                }
            }
        }
    }
}

void _player::renderFrameItpWithGLCmds(int n, float interp, const struct md2_model_t* mdl)
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
    if (actionTrigger == ACT_JUMP) {
        int mid = (startFrame + endFrame) / 2;
        if (vy > 0 && n >= mid) {
            next = mid;
        } else if (vy <= 0 && n >= endFrame) {
            next = endFrame;
        } else {
            next = std::min(n + 1, endFrame);
        }
    }
    else {
        next = (n < endFrame) ? n + 1 : startFrame;
    }

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
