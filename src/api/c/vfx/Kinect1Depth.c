/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include "kinect1.h"
#include "vfx.h"
#include "OpenISSPipeline.h"
#include "vfx/Kinect1Depth.h"

t_iss_vfx_ops g_tVFXKinect1DepthOps;

// back: owned by libfreenect (implicit for depth)
// mid: owned by callbacks, "latest frame ready"
// front: owned by GL, "currently being drawn"
extern uint8_t *depth_mid, *depth_front;

extern GLuint gl_depth_tex;

extern GLfloat camera_angle;

extern int camera_rotate;

extern freenect_device *f_dev;


void vfx_kinect1_depth_DrawGLScene()
{
	kinect1_receive_rgb_depth_frames();
	
	glBindTexture(GL_TEXTURE_2D, gl_depth_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, depth_front);

	if(camera_rotate)
	{
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    freenect_raw_tilt_state* state;
	    freenect_update_tilt_state(f_dev);
	    state = freenect_get_tilt_state(f_dev);
	    GLfloat x_accel_raw, x_accel,y_accel_raw,y_accel;
	    x_accel_raw = (GLfloat)state->accelerometer_x/819.0;
	    y_accel_raw = (GLfloat)state->accelerometer_y/819.0;

	    // sloppy acceleration vector cleanup
	    GLfloat accel_length = sqrt(x_accel_raw * x_accel_raw + y_accel_raw * y_accel_raw);
	    x_accel = x_accel_raw/accel_length;
	    y_accel = y_accel_raw/accel_length;
	    camera_angle = atan2(y_accel,x_accel)*180/M_PI -90.0;
	}
	else
	{
		camera_angle = 0;
	}

	glLoadIdentity();

	glPushMatrix();
	  glTranslatef((640.0/2.0),(480.0/2.0) ,0.0);
	  glRotatef(camera_angle, 0.0, 0.0, 1.0);
	  glTranslatef(-(640.0/2.0),-(480.0/2.0) ,0.0);
	  glBegin(GL_TRIANGLE_FAN);
	    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	    glTexCoord2f(0, 1); glVertex3f(0,0,1.0);
	    glTexCoord2f(1, 1); glVertex3f(640,0,1.0);
	    glTexCoord2f(1, 0); glVertex3f(640,480,1.0);
	    glTexCoord2f(0, 0); glVertex3f(0,480,1.0);
	  glEnd();
	glPopMatrix();

	/*
	glBindTexture(GL_TEXTURE_2D, gl_rgb_tex);
	
	if (current_format == FREENECT_VIDEO_RGB || current_format == FREENECT_VIDEO_YUV_RGB)
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb_front);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, 1, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rgb_front+640*4);

	glPushMatrix();
	  glTranslatef(640+(640.0/2.0),(480.0/2.0) ,0.0);
	  glRotatef(camera_angle, 0.0, 0.0, 1.0);
	  glTranslatef(-(640+(640.0/2.0)),-(480.0/2.0) ,0.0);

	  glBegin(GL_TRIANGLE_FAN);
	    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	    glTexCoord2f(0, 1); glVertex3f(640,0,0);
	    glTexCoord2f(1, 1); glVertex3f(1280,0,0);
	    glTexCoord2f(1, 0); glVertex3f(1280,480,0);
	    glTexCoord2f(0, 0); glVertex3f(640,480,0);
	  glEnd();
	glPopMatrix();
	glutSwapBuffers();
	*/
}

void vfx_kinect1_depth_keyPressed(unsigned char key, int x, int y)
{
}

int vfx_kinect1_depth_module_init()
{
    g_tVFXKinect1DepthOps.vfx_init = &vfx_kinect1_depth_init;
    g_tVFXKinect1DepthOps.vfx_draw = &vfx_kinect1_depth_draw;
    g_tVFXKinect1DepthOps.vfx_free = &vfx_kinect1_depth_free;
    
    g_apvfx[VFX_KINECT1_DEPTH] = &g_tVFXKinect1DepthOps;
    
    return 0;
}

int vfx_kinect1_depth_init()
{
    printf("vfx_kinect1_depth_init()\n");
    return 0;
}

void vfx_kinect1_depth_free()
{
    printf("vfx_kinect1_depth_free()\n");
}

void vfx_kinect1_depth_draw()
{
    vfx_kinect1_depth_DrawGLScene();
}

#ifdef MAIN_VFX_TEST
int main(int argc, char** argv)
{
    g_tVFXKinect1DepthOps.vfx_init = &vfx_kinect1_depth_init;
    g_tVFXKinect1DepthOps.vfx_draw = &vfx_kinect1_depth_draw;
    g_tVFXKinect1DepthOps.vfx_free = &vfx_kinect1_depth_free;

    vfx_kinect1_depth_init();
    vfx_kinect1_depth_draw();
    vfx_kinect1_depth_free();
    return 0;
}
#endif
