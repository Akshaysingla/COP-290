#include <GL/glut.h>
#include <GL/glui.h>
#include "Structures.h"
#include "Variables.h"
#include "Menu.h"
#include <pthread.h>
#include "Display.h"
#include <stdlib.h>
#include <stdio.h>

GLUI *sub_win;

void create_menu ( GLUI *glui ) {               //Creates The User Interface
    sub_win = glui;
    glui->add_statictext( "" );
    
    GLUI_StaticText *st = glui->add_statictext( "  SCREEN SAVER" );
    st->set_font ( GLUT_BITMAP_HELVETICA_18 );
    st->set_alignment ( GLUI_ALIGN_CENTER );
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    GLUI_StaticText *st2 = glui->add_statictext( "ID Of Selected Ball" );       //Selects a Ball For Changing Velocity
    st2->set_font ( GLUT_BITMAP_HELVETICA_18  );
    st2->set_alignment ( GLUI_ALIGN_CENTER );
    
    GLUI_Spinner *select_ball = glui->add_spinner ( "", GLUI_SPINNER_INT, &selball );
    select_ball -> set_font ( GLUT_BITMAP_9_BY_15 );
    select_ball -> set_int_limits ( 0, n+1, GLUI_LIMIT_WRAP );
    select_ball -> set_alignment (  GLUI_ALIGN_CENTER );
    select_ball -> set_speed ( 0.25 );
    
    glui->add_statictext( "" );
    
    GLUI_Button *des = glui->add_button ( "Deselect Ball", 0, deselect );       // Deselects a Ball
    des->set_alignment (  GLUI_ALIGN_CENTER );
    des->set_font (  GLUT_BITMAP_9_BY_15 );
    des->set_h ( 20 );
    des-> set_w ( 450 );
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    GLUI_StaticText *st3 = glui->add_statictext( "Change Speed Of Selected Ball" );
    st3->set_font ( GLUT_BITMAP_HELVETICA_18 );
    st3->set_alignment ( GLUI_ALIGN_CENTER );
    
    glui->add_statictext( "" );
    
    GLUI_Button *vel_inc = glui->add_button ( "Increase Speed by 10 percent", 0, increase_vel );
    vel_inc->set_alignment (  GLUI_ALIGN_CENTER );
    vel_inc->set_font (  GLUT_BITMAP_9_BY_15 );
    vel_inc->set_h ( 20 );
    vel_inc-> set_w ( 450 );
    
    GLUI_Button *vel_dec = glui->add_button ( "Decrease Speed by 10 percent", 0, decrease_vel );
    vel_dec->set_alignment (  GLUI_ALIGN_CENTER );
    vel_dec->set_font (  GLUT_BITMAP_9_BY_15 );
    vel_dec->set_h ( 20 );
    vel_dec-> set_w ( 450 );
    
    GLUI_Button *vel_stop = glui->add_button ( "Stop The Ball", 0, stop_vel );
    vel_stop->set_alignment (  GLUI_ALIGN_CENTER );
    vel_stop->set_font (  GLUT_BITMAP_9_BY_15 );
    vel_stop->set_h ( 20 );
    vel_stop-> set_w ( 450 );
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    sph_rot = new GLUI_Rotation(glui, "Rotate",view_rotate);            //Rotate the Cube : Only Available In 3D Mode
    sph_rot->set_spin(0.1);
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    GLUI_Panel *view_select = glui->add_panel ( "Select Your View", GLUI_PANEL_EMBOSSED );
    view_select -> set_font ( GLUT_BITMAP_HELVETICA_18 );
    glui->add_statictext_to_panel ( view_select, "" );
    GLUI_RadioGroup *view = glui->add_radiogroup_to_panel ( view_select , &dimension, 0, change_view );
    GLUI_RadioButton *button_2D = glui->add_radiobutton_to_group ( view, "  2D" );          //Moves to 3D Mode
    button_2D -> set_font ( GLUT_BITMAP_9_BY_15 );
    button_2D -> set_alignment (  GLUI_ALIGN_CENTER );
    GLUI_RadioButton *button_3D = glui->add_radiobutton_to_group ( view, "  3D" );          //Moves to 2D Mode
    button_3D -> set_font ( GLUT_BITMAP_9_BY_15 );
    button_3D -> set_alignment (  GLUI_ALIGN_CENTER );
    glui->add_statictext_to_panel ( view_select, "" );
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    GLUI_StaticText *st4 = glui->add_statictext( "Zoom In / Out" );                     //Slider To Zoom In/Zoom Out Of the View
    st4->set_font ( GLUT_BITMAP_HELVETICA_18 );
    st4->set_alignment ( GLUI_ALIGN_CENTER );
    
    glui->add_statictext( "" );
    
    GLUI_Scrollbar *zoom;
    zoom = new GLUI_Scrollbar ( glui, "", GLUI_SCROLL_HORIZONTAL, &alpha, 0, zoom_func );
    zoom -> set_font ( GLUT_BITMAP_9_BY_15 );
    zoom -> set_float_limits ( 1.5, 6, GLUI_LIMIT_CLAMP );
    zoom -> set_alignment (  GLUI_ALIGN_CENTER );
    zoom -> set_speed ( 0.5 );
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    GLUI_Listbox *background = new GLUI_Listbox(glui,"Background :" , &background_num);         //Drop-Down To Choose Background
    background->set_font ( GLUT_BITMAP_HELVETICA_18 );
    background -> set_alignment (  GLUI_ALIGN_CENTER );
    background->add_item(0,"Underwater");
    background->add_item(1,"Space");
    background->add_item(2,"Space : 2");
    background->add_item(3,"Abstract ");
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    GLUI_Listbox *boxtexture = new GLUI_Listbox(glui,"Glass :    " , &boxtexture_num);          //Drop-Down To Choose Glass
    boxtexture->set_font ( GLUT_BITMAP_HELVETICA_18 );
    boxtexture -> set_alignment (  GLUI_ALIGN_CENTER );
    boxtexture->add_item(4,"Box Glass");
    boxtexture->add_item(5,"Clear Glass");
    boxtexture->add_item(6,"Pattern");
    boxtexture->add_item(7,"Caged");
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    GLUI_Button *ss = glui->add_button ( "View Your Customized Screensaver", 0, screen_saver );     //Moves On to Full-Screen
    ss->set_alignment (  GLUI_ALIGN_CENTER );
    ss->set_font (  GLUT_BITMAP_9_BY_15 );
    ss->set_h ( 20 );
    ss-> set_w ( 450 );

    GLUI_StaticText *st5 = glui->add_statictext( "(Press any key to exit)" );
    st5->set_alignment ( GLUI_ALIGN_CENTER );
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    GLUI_Checkbox *chk = glui->add_checkbox ( " Deselect to Pause", &moving );                 //  To pause/Unpause the Motion
    chk->set_font ( GLUT_BITMAP_HELVETICA_18 );
    chk->set_alignment (  GLUI_ALIGN_CENTER );
    
    glui->add_statictext( "" );
    glui->add_separator();
    glui->add_statictext( "" );
    
    GLUI_Button *ex = glui->add_button ( "EXIT", 0, exit_func );
    ex->set_alignment (  GLUI_ALIGN_CENTER );
    ex->set_font (  GLUT_BITMAP_9_BY_15 );
    ex->set_h ( 20 );
    ex-> set_w ( 450 );
}

void screen_saver ( int i ) {
    if ( dimension == 1 ) {             //Moves
        screen_mode = true;
        sub_win -> hide ();
        glViewport ( 200-(global_w/2), -(global_h/2), 2*global_w, 2*global_h);
    }
    if ( dimension == 0 ) {
        screen_mode = true;
        sub_win->hide ();
        glViewport ( 200-2.5*(global_w/2), -2.5*(global_h/2), 3.5*global_w, 3.5*global_h);
    }
}

void exit_func ( int i ) {
    exit(0);
}

void zoom_func ( int i ) {
    glViewport ( 500+(global_w/2.0)*(1-alpha), (global_h/2.0)*(1-alpha), alpha*global_w, alpha*global_h );
}

void increase_vel ( int i ) {
    message m;
    m.state = 3;
    if ( selball )
        mail[selball-1].push ( m );         //Sends Message :3 to selected ball, Increases speed by 10%
}
void decrease_vel ( int i ) {
    message m;
    m.state = 4;                           //Sends Message :4 to selected ball, Decrease speed by 10%
    if ( selball )
        mail[selball-1].push ( m );
}

void stop_vel ( int i ) {
    message m;
    m.state = 5;
    if ( selball )
        mail[selball-1].push ( m );         //Sends Message :4 to selected ball, Speed = 0
}

void deselect ( int i ) {
    selball = 0;
}

void change_view ( int j ) {
    if ( dimension == 0 ) {
        sph_rot -> disable ();
        int num_iter = 250;
        float orig[16];
        moving = false;
        for ( int i = 0; i < 16; i++ )
            orig[i] = view_rotate[i];
        for ( int k = 0; k < num_iter; k++ ) {          //Rototes the View From 3D to 2D
            for ( int i = 0; i < 16; i++ ) {
                if ( i % 5 != 0 )
                    view_rotate[i] -= (orig[i]/num_iter);
            }
            for ( int i = 0; i < 16; i += 5 ) {
                view_rotate[i] += ((1-orig[i])/num_iter);
            }
            display ();
        }
        for ( int i = 0; i < 16; i++ ) {
            if ( i % 5 != 0 )
                view_rotate[i] = 0;
        }
        for ( int i = 0; i < 16; i += 5 ) {
            view_rotate[i] = 1;
        }
        for ( int i = 0; i < n; i++ ) {
            message m;
            m.state = 6;
            pthread_mutex_lock ( &mtx );
            mail[i].push ( m );
            pthread_mutex_unlock ( &mtx );
        }
        moving = true;
        
    }
    else if ( dimension == 1 ) {            //Roates the View from 3D to 2D
        sph_rot -> enable ();
        int num_iter = 250;
        moving = false;
        for ( int k = 0; k < num_iter; k++ ) {
            for ( int i = 0; i < 16; i++ ) {
                if ( i % 5 != 0 )
                    view_rotate[i] += (view_rotate_init[i]/num_iter); 
            }
            for ( int i = 0; i < 16; i += 5 ) {
                view_rotate[i] += ((view_rotate_init[i] - 1)/num_iter); 
            }
            display ();
        }
        for ( int i = 0; i < 16; i++ )
            view_rotate[i] = view_rotate_init[i];
        for ( int i = 0; i < n; i++ ) {
            message m;
            m.state = 7;
            pthread_mutex_lock ( &mtx );
            mail[i].push ( m );
            pthread_mutex_unlock ( &mtx );
        }
        moving = true;
    }
}
