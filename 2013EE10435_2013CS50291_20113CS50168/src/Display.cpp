#include <iostream>
#include <GL/glut.h>
#include <GL/glui.h>
#include <vector>
#include "Variables.h"
#include "Structures.h"
#include "Menu.h"
#include "SDL/SDL.h"
#include <SDL/SDL_image.h>

GLuint texture[8];

GLUI *sub_w;

void LoadGLTextures( ) {
    /* Create storage space for the texture */
    SDL_Surface *TextureImage[9];
    const char* names[8] = {"resources/image/back1.jpg","resources/image/back2.jpg","resources/image/back3.jpg","resources/image/back4.jpg","resources/image/glass.jpg",
        "resources/image/glass2.jpg","resources/image/glass3.jpg","resources/image/glass4.jpg"};
    /* Load The Bitmap into Memory */
    for (int i = 0 ; i< 8 ; i++){
        if ((TextureImage[0] = IMG_Load(names[i]))) {
            glGenTextures( 1, &texture[i] );
            glBindTexture( GL_TEXTURE_2D, texture[i] );
            glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage[0]->w,
                         TextureImage[0]->h, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, TextureImage[0]->pixels );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        }
        
        /* Free up some memory */
        if ( TextureImage[0] )
            SDL_FreeSurface( TextureImage[0] );
    }
}

void init_material(float Ka, float Kd, float Ks, float Kp, float Mr, float Mg, float Mb) {
    // Material variables
    float ambient[] = { Ka * Mr, Ka * Mg, Ka * Mb, 1.0 };
    float diffuse[] = { Kd * Mr, Kd * Mg, Kd * Mb, 1.0 };
    float specular[] = { Ks * Mr, Ks * Mg, Ks * Mb, 1.0 };
    
    // Initialize material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);       //Load Ambient Properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);       //Load Diffuse Properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);     //Load Specular Properties
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Kp);           //Load Shininess Properites
}

void reshape(GLint w, GLint h) {
    int x, y;
    GLUI_Master.get_viewport_area ( &x, &y, &w , &h );
    global_w = w;
    global_h = h;
    glViewport( 500-w/2, -h/2, 2*w, 2*h );
    GLfloat aspect = (GLfloat)w / (GLfloat)h;
    asp = aspect;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1500.0*aspect, 1500.0*aspect, -1550.0, 1500.0, -5500.0, 5500.0);
}

void init_light(int light_source, float Lx, float Ly, float Lz, float Lr, float Lg, float Lb) {
    // Light variables
    float light_position[] = { Lx, Ly, Lz, 0.0 };
    float light_color[] = { Lr, Lg, Lb, 1.0 };
    
    // Initialize light source
    glEnable(GL_LIGHTING);
    glEnable(light_source);
    glLightfv(light_source, GL_POSITION, light_position);
    glLightfv(light_source, GL_AMBIENT, light_color);
    glLightfv(light_source, GL_DIFFUSE, light_color);
    glLightfv(light_source, GL_SPECULAR, light_color);
    glLightf(light_source, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(light_source, GL_LINEAR_ATTENUATION, 0.0);
    glLightf(light_source, GL_QUADRATIC_ATTENUATION, 0.0);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void drawCube() {
    
    glBegin(GL_QUADS);
    /* Front Face */
    glTexCoord2f( 0.0, 1 ); glVertex3f( -CUBESIDE/2, -CUBESIDE/2, CUBESIDE/2 );
    glTexCoord2f( 1, 1 ); glVertex3f(  CUBESIDE/2, -CUBESIDE/2, CUBESIDE/2 );
    glTexCoord2f( 1, 0.0 ); glVertex3f(  CUBESIDE/2,  CUBESIDE/2, CUBESIDE/2 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -CUBESIDE/2,  CUBESIDE/2, CUBESIDE/2 );
    
    /* Back Face */
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -CUBESIDE/2, -CUBESIDE/2, -CUBESIDE/2 );
    glTexCoord2f( 0.0, 1 ); glVertex3f( -CUBESIDE/2,  CUBESIDE/2, -CUBESIDE/2 );
    glTexCoord2f( 1, 1 ); glVertex3f(  CUBESIDE/2,  CUBESIDE/2, -CUBESIDE/2 );
    glTexCoord2f( 1, 0.0 ); glVertex3f(  CUBESIDE/2, -CUBESIDE/2, -CUBESIDE/2 );
    
    /* Top Face */
    glTexCoord2f( 1, 1 ); glVertex3f( -CUBESIDE/2,  CUBESIDE/2, -CUBESIDE/2 );
    glTexCoord2f( 1, 0.0 ); glVertex3f( -CUBESIDE/2,  CUBESIDE/2,  CUBESIDE/2 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  CUBESIDE/2,  CUBESIDE/2,  CUBESIDE/2 );
    glTexCoord2f( 0.0, 1 ); glVertex3f(  CUBESIDE/2,  CUBESIDE/2, -CUBESIDE/2 );
    
    /* Bottom Face */
    glTexCoord2f( 0.0, 1 ); glVertex3f( -CUBESIDE/2, -CUBESIDE/2, -CUBESIDE/2 );
    glTexCoord2f( 1, 1 ); glVertex3f(  CUBESIDE/2, -CUBESIDE/2, -CUBESIDE/2 );
    glTexCoord2f( 1, 0.0 ); glVertex3f(  CUBESIDE/2, -CUBESIDE/2,  CUBESIDE/2 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -CUBESIDE/2, -CUBESIDE/2,  CUBESIDE/2 );
    
    /* Right face */
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( CUBESIDE/2, -CUBESIDE/2, -CUBESIDE/2 );
    glTexCoord2f( 0.0, 1 ); glVertex3f( CUBESIDE/2,  CUBESIDE/2, -CUBESIDE/2 );
    glTexCoord2f( 1, 1 ); glVertex3f( CUBESIDE/2,  CUBESIDE/2,  CUBESIDE/2 );
    glTexCoord2f( 1, 0.0 ); glVertex3f( CUBESIDE/2, -CUBESIDE/2,  CUBESIDE/2 );
    
    /* Left Face */
    glTexCoord2f( 1, 0.0 ); glVertex3f( -CUBESIDE/2, -CUBESIDE/2, -CUBESIDE/2 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -CUBESIDE/2, -CUBESIDE/2,  CUBESIDE/2 );
    glTexCoord2f( 0.0, 1 ); glVertex3f( -CUBESIDE/2,  CUBESIDE/2,  CUBESIDE/2 );
    glTexCoord2f( 1, 1 ); glVertex3f( -CUBESIDE/2,  CUBESIDE/2, -CUBESIDE/2 );
    glEnd( );
}

void draw_background(){
    glBegin(GL_QUADS);
    glTexCoord3d(0.0,0.0, -1); glVertex3f(-global_w, -global_h, -global_h);
    glTexCoord3d(1.0,0.0, -1); glVertex3f(global_w, -global_h, -global_h);
    glTexCoord3d(1.0,1.0, -1); glVertex3f(global_w, global_h, -global_h);
    glTexCoord3d(0.0,1.0, -1); glVertex3f(-global_w, global_h, -global_h);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear Last Screen
    if ( begin1 ) {
        glEnable( GL_TEXTURE_2D );
        glDisable(GL_LIGHTING);                     //Disable Lighting to Draw Textured Objects
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-1500.0*asp, 1500.0*asp, -1550.0, 1500.0, -5500.0, 5500.0);
        glScalef(1, -1, 1);                         //Reivert Along Y-axis to amke
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glBindTexture( GL_TEXTURE_2D, texture[background_num] );  //Load texture According to the input from the GLUI
        draw_background();                                          //Draw Background
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glLoadIdentity();
        glOrtho(-1500.0*asp, 1500.0*asp, -1550.0, 1500.0, -5500.0, 5500.0);
        if ( dimension == 0 && screen_mode == true ){
            glTranslatef ( 0, -24, 0 );
        }
        glMultMatrixf(view_rotate);
        glMatrixMode(GL_MODELVIEW);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glBindTexture( GL_TEXTURE_2D, texture[boxtexture_num] );
        glEnable(GL_BLEND);                 // Turn Blending On
        glDisable(GL_DEPTH_TEST);           // Turn Depth Testing Off
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);   // Blend to Make The Cube Transparent
        drawCube();                         //Draw The Glass Cube
        glDisable(GL_BLEND);                // Turn Blending Off
        glEnable(GL_DEPTH_TEST);           // Turn Depth Testing Off
        glDisable( GL_TEXTURE_2D );
        glEnable(GL_LIGHTING);              //Enable Lighting
        init_material(1,1,1,1,1,1,1);       // Make Cube Boundary
        glutWireCube(CUBESIDE);
        for (int i = 0; i < n; i++)
        {
            glPushMatrix();
            glTranslatef(fordisp[i].x,fordisp[i].y,fordisp[i].z);
            if( ifcollide[i]>0) {               //If Ball Was Involved In a Collision
                if ( ifcollide[i] > 10 )
                    init_material(1, 0, 1, 128 , 1 , 0, 0 );    // Makes It Red for the first 10 iterations
                else {
                    GLfloat diff = 5 - ifcollide[i];                //Fades the Color
                    init_material(Ka, Kd, Ks, 128, 1 - (diff*((1-fordisp[i].r)/5)), diff*(fordisp[i].g/5) , diff*(fordisp[i].b/5) );
                }
                ifcollide[i]--;
            }
            else
                init_material(Ka, Kd, Ks, 100 * Kp, fordisp[i].r, fordisp[i].g,fordisp[i].b);       //Loads the Material Property For the Sphere with Stored Colors
            glutSolidSphere(fordisp[i].rad,40,40);
            
            if ( i+1 == selball ) {                             //If the ball is selected
                init_material(Ka, Kd, Ks, 100 * Kp, 0, 0, 0);
                glutWireSphere ( fordisp[i].rad+4, 20, 20);      //Forms a Wire Sphere around the sphere
            }
            glPopMatrix();
        }
        
    }
    glutSwapBuffers();
}

void init() {
    glLoadIdentity();               //clear transformations
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);                        //To add Material Properties
    init_light(GL_LIGHT0, 0, 1, 1, 1, 1, 1);        //Initialize White Lighting across all planes
    init_light(GL_LIGHT1, 1, 0, 1, 1, 1, 1);
    init_light(GL_LIGHT2, 1, 1, 0, 1, 1, 1);
    LoadGLTextures();                               //Load Textures For Background and Glass Cube
    
}

void timer(int v) {
    if (moving) {                                   //changes display if moving
        glutPostRedisplay();
    }
    glutTimerFunc( 0, timer, v);
}

void myGlutIdle( void ) {                           //Idle Function
    glutSetWindow(window_id);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
    switch (key){
        default :          //To exit ScreenSaver Mode and Make The UI controls appear
            if ( screen_mode ) {
                sub_w -> show ();
                glViewport ( 500+(global_w/2.0)*(1-alpha), (global_h/2.0)*(1-alpha), alpha*global_w, alpha*global_h );
                screen_mode = false;
            }
            break;
    }
}

void start ( int i ) {
    begin1 = true;
    GLUI *glui_sub = GLUI_Master.create_glui_subwindow( window_id, GLUI_SUBWINDOW_LEFT );
    sub_w = glui_sub;
    create_menu ( glui_sub );
    glui_sub -> set_main_gfx_window ( window_id );
}

void *maindisplay ( void *id ) {
    glutInit(&c, v);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1920, 1080);
    window_id = glutCreateWindow("ScreenSaver");            //initialize window
    glutFullScreen();                                       //Enter Full Screen Mode
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutTimerFunc(10, timer, 0);
    start ( 0 );
    GLUI_Master.set_glutIdleFunc ( myGlutIdle );
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
