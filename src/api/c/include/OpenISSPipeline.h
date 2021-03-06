#ifndef _OPENISS_PIPELINE_H
#define _OPENISS_PIPELINE_H

#include "vfx.h"

typedef enum
{
    VFX_TEST_START = 0,
    VFX_CONTOUR,
    VFX_KINECT1_DEPTH,
    VFX_KINECT1_RGB,
    VFX_KINECT1_PCL,
    VFX_END_CLOSING
} e_vfx_enum;

typedef struct
{
    e_vfx_enum m_eCurrentEffect;
    
    // In frames
    int m_iTime;
}
t_iss_state;

extern t_iss_state g_tISSStage;

extern t_iss_vfx_ops* g_apvfx[VFX_END_CLOSING + 1];

int iss_init(void);
void iss_draw(void);
void iss_free(void);

#endif
