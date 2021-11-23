#include <windows.h>
#include <GL/glut.h>
#include<bits/stdc++.h>
#include <dos.h>
#include <iostream>
using namespace std ;

#define ortho_max 500
#define ortho_min -200



class sh_clipping
{
private:
    float clipper[2][2],polygon[100][2], res[100][2],temp[100][2];
    int poly_no, res_no=0,temp_no ;
    char order[3];
public :
    void set_data()
    {

        ifstream flie_read;
        flie_read.open("input5.txt");


        float f ;
        flie_read >> f ;
        clipper[0][0] = f  ;
        //cout << f << "  "  << clipper[0][0] ;


        flie_read >> f ;
        clipper[1][0] = f ;
        //cout << f << "  "  << clipper[1][0] ;

        flie_read >> f ;
        clipper[0][1] = f ;

        flie_read >> f ;
        clipper[1][1] = f ;

        //cout << clipper[0][0] << "   "<< clipper[1][0] << "   "<< clipper[0][1] << "   "<< clipper[1][1] << endl <<endl  <<endl ;

        for(int i=0 ; i<4 ; i++)
        {
            flie_read >> order[i] ;
        }

        flie_read >> poly_no ;

        for( int i=0 ; i<poly_no ; i++)
        {
            flie_read >> polygon[i][0];
            flie_read >> polygon[i][1];
        }
        flie_read.close();
    }

    void show_data()
    {

        cout << "Xmin : " << clipper[0][0]  <<endl ;
        cout << "Xmax : " << clipper[1][0]  <<endl ;
        cout << "Ymin : " << clipper[0][1]  <<endl ;
        cout << "Ymax : " << clipper[1][1]  <<endl ;

        cout << "clipping edge sequence : " ;

        for(int i=0 ; i<4 ; i++)
        {
            cout << order[i] << "\t" ;
        }

        cout <<endl << "no of vertices in polygon : " << poly_no << endl ;

        for(int i=0 ; i<poly_no ; i++)
        {
            cout <<"P" << i+1 << " : ("<<polygon[i][0] <<","<<polygon[i][1]<< ")"<<endl;
        }
    }

    void draw_clipper()
    {
        glColor3f(1.0, 1.0, 1.0);
        for(int i=0 ; i<2 ; i++)
        {
            glBegin(GL_LINES);
            glVertex2f(clipper[i][0], ortho_min);
            glVertex2f(clipper[i][0], ortho_max);
            glEnd();
            glBegin(GL_LINES);
            glVertex2f(ortho_min,clipper[i][1] );
            glVertex2f(ortho_max,clipper[i][1] );
            glEnd();
        }
        glFlush();
    }

    void draw_poly()
    {
        glColor3f(0, 1.0, 0);
        glBegin(GL_LINE_LOOP);

        //glVertex2f(polygon[0][0], polygon[0][1]);
        for(int i=0 ; i<poly_no ; i++)
        {

            glVertex2f(polygon[i][0], polygon[i][1]);
            cout << polygon[i][0]<< "  " <<  polygon[i][1] << endl ;

        }
        //glVertex2f(polygon[0][0], polygon[0][1]);
        //glVertex2f(polygon[8][0], polygon[8][1]);
        glEnd();
        glFlush();


    }

    void draw_poly_clipped(float r,float g,float b,bool poly)
    {
        glColor3f(r, g, b);
        if(poly)
            glBegin(GL_POLYGON);
        else
            glBegin(GL_LINE_LOOP);


        for(int i=0 ; i<res_no ; i++)
        {

            glVertex2f(res[i][0], res[i][1]);

        }
        // glVertex2f(res[8][0], res[8][1]);
        glEnd();
        glFlush();
    }

    void show_res()
    {
        for(int i=0 ; i<res_no ; i++)
        {
            cout << "(" << res[i][0] << "," << res[i][1] <<  ")\t" ;
        }
    }
    void show_temp()
    {
        for(int i=0 ; i<temp_no ; i++)
        {
            cout << "\t(" << temp[i][0] << "," << temp[i][1] <<  ")" ;
        }
    }

    void initial_res()
    {
        res_no = poly_no ;
        for(int i=0 ; i<poly_no ; i++)
        {
            res[i][0] = polygon[i][0];
            res[i][1] = polygon[i][1];
        }
    }
    void update_temp()
    {
        temp_no = res_no ;
        for(int i=0 ; i<res_no ; i++)
        {
            temp[i][0] = res[i][0];
            temp[i][1] = res[i][1];
        }
    }
    float slope(float a1, float b1, float a2, float b2  )
    {
        return (b2-b1)/(a2-a1);
    }
    void keep_2nd_point(int j)
    {
        res[res_no][0] = temp[j][0];
        res[res_no][1] = temp[j][1] ;
        res_no++;
    }

    void intersection_point_x(float m, float y, float x1, float y1)
    {
        //cout << "(" << m << " " << y << " " << x1 << " " << y1 << ")" ;
        res[res_no][0] = x1+m*(y-y1);
        res[res_no][1] = y ;
        res_no++;
    }
    void intersection_point_y(float m, float x, float x1, float y1)
    {
        res[res_no][0] = x;
        res[res_no][1] = y1+m*(x-x1);
        res_no++;
    }

    void remove_dublicate()
    {
        for( int i=0 ; i<res_no ; i++ )
        {
            int j = (i+1)%res_no;
            if(res[i][0] == res[j][0] && res[i][1] == res[j][1])
            {
                for( int k=j ; k<res_no ; k++)
                {
                    int l = (k+1)%res_no;
                    res[k][0] = res[l][0] ;
                    res[k][1] = res[l][1] ;
                }
                res_no-- ;
                i-- ;
                if(j==0)
                    i = res_no - 2 ;
            }
        }
    }

//*
    void clip_top()
    {
        res_no = 0 ;
        for( int i=0 ; i<temp_no ; i++)
        {
            int j = (i+1)%temp_no ;

            float m = slope(temp[i][1],temp[i][0],temp[j][1],temp[j][0]);   //(y1,x1,y2,x2)

            if(temp[i][1] <= clipper[1][1] && temp[j][1] <= clipper[1][1]) //y1 <= ymax && y2 <= ymax
            {
                // cout << "  a " << j ;
                keep_2nd_point(j);
            }
            if(temp[i][1] > clipper[1][1] && temp[j][1] <= clipper[1][1]) //y1 > ymax && y2 <= ymax
            {
                // cout << "  b " << j ;
                intersection_point_x(m,clipper[1][1], temp[i][0], temp[i][1] );   //x1+m*(ymax-y1)
                keep_2nd_point(j);
            }
            if(temp[i][1] <= clipper[1][1]  && temp[j][1] > clipper[1][1]) //y1 <= ymax  && y2 > ymax
            {
                // cout << "  c " << j ;
                intersection_point_x(m,clipper[1][1], temp[i][0], temp[i][1] );
            }
        }
    }
    void clip_bottom()
    {
        res_no = 0 ;
        for( int i=0 ; i<temp_no ; i++)
        {
            int j = (i+1)%temp_no ;

            float m = slope(temp[i][1],temp[i][0],temp[j][1],temp[j][0]);   //(x1,y1,x2,y2)
            //cout << m << "\t"  ;

            if(temp[i][1] >= clipper[0][1] && temp[j][1] >= clipper[0][1]) //y1 >= ymin && y2 >= ymin
            {
                keep_2nd_point(j);
            }
            if(temp[i][1] < clipper[0][1] && temp[j][1] >= clipper[0][1])  //y1 < ymin && y2 >= ymin
            {
                intersection_point_x(m,clipper[0][1], temp[i][0], temp[i][1] );  //x1+m*(ymin-y1)
                keep_2nd_point(j);
            }
            if(temp[i][1] >= clipper[0][1]  && temp[j][1] < clipper[0][1])   //y1 >= ymin  && y2 < ymin
            {
                intersection_point_x(m,clipper[0][1], temp[i][0], temp[i][1] );   //x1+m*(ymin-y1)
            }
        }
    }
    void clip_left()
    {
        res_no = 0 ;
        for( int i=0 ; i<temp_no ; i++)
        {
            int j = (i+1)%temp_no ;

            float m =  slope(temp[i][0],temp[i][1],temp[j][0],temp[j][1])   ;    // (x1,y1,x2,y2)
            //cout << m << "\t"  ;

            if(temp[i][0] >= clipper[0][0] && temp[j][0] >= clipper[0][0])       //x1 >= xmin && x2 >= xmin
            {
                keep_2nd_point(j);
            }
            if(temp[i][0]  < clipper[0][0] && temp[j][0] >= clipper[0][0])       //(x1 < xmin && x2 >= xmin)
            {
                intersection_point_y(m,clipper[0][0], temp[i][0], temp[i][1] );     //y1+m*(xmin-x1);
                keep_2nd_point(j);
            }
            if(temp[i][0]  >= clipper[0][0]  && temp[j][0] < clipper[0][0])  //(x1 >= xmin  && x2 < xmin)
            {
                intersection_point_y(m,clipper[0][0], temp[i][0], temp[i][1] );        //y1+m*(xmin-x1);
            }
        }
    }

    void clip_right()
    {
        res_no = 0 ;
        for( int i=0 ; i<temp_no ; i++)
        {
            int j = (i+1)%temp_no ;
            float m =  slope(temp[i][0],temp[i][1],temp[j][0],temp[j][1]) ;     // (x1,y1,x2,y2)
            //cout << m << "\t"  ;

            if(temp[i][0]<= clipper[1][0] && temp[j][0] <= clipper[1][0])       //1 <= xmax && x2 <= xmax
            {
                keep_2nd_point(j);
            }
            if(temp[i][0]  > clipper[1][0] && temp[j][0] <= clipper[1][0])       //(x1 > xmax && x2 <= xmax
            {
                intersection_point_y(m,clipper[1][0], temp[i][0], temp[i][1] );     //y1+m*(xmax-x1);
                keep_2nd_point(j);
            }
            if(temp[i][0] <= clipper[1][0]  && temp[j][0] > clipper[1][0])  //x1 <= xmax  && x2 > xmax
            {
                intersection_point_y(m,clipper[1][0], temp[i][0], temp[i][1] );        //y1+m*(xmax-x1);
            }
        }
    }

    void suthod()
    {
        initial_res();

        for( int i=0 ; i<4 ; i++)
        {
            Sleep(1000);
            float r=0, g=0, b=0  ;
            update_temp();
            cout << "Clipping with respect to " ;
            if(order[i]=='T')
            {
                r =1 ;
                g = 1 ;
                cout << "Top edge..."<<endl ;
                clip_top();
                draw_poly_clipped(1, 0,0,false );

            }
            else if(order[i]=='B')
            {
                b = 1 ;
                cout << "Bottom edge..." <<endl ;
                clip_bottom() ;
                draw_poly_clipped(0, 0,1,false );

            }
            else if(order[i]=='R')
            {

                r = 1   ;
                b = 1 ;
                cout << "Right edge..." <<endl ;
                clip_right();
                draw_poly_clipped(1, 0,1,false );

            }
            else if(order[i]=='L')
            {
                cout << "Left edge..." <<endl ;
                clip_left();
                draw_poly_clipped(1, 1,0,false );
            }
            else
            {
                cout << "Sequence error" << endl ;
            }
            cout << "New Sequence :\t" ;
            remove_dublicate();
            show_res();
            cout << endl << endl ;
        }
        Sleep(1000);
        draw_poly_clipped(1, 1,1,true );

    }

    void check_amni_e()
    {
        ifstream flie_read;
        flie_read.open("input.txt");
        float x ;
        flie_read >> x ;
        cout << x;
        flie_read >> x ;
        cout << x;
        flie_read >> x ;
        cout << x;
        flie_read >> x ;
        cout << x;
        cout << endl<<endl;
        flie_read.close();
        //cin >>x ;
    }




};

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(1.0);
//glMatrixMode(GL_PROJECTION);
    gluOrtho2D(ortho_min,ortho_max, ortho_min, ortho_max);
    // glLoadIdentity();
}

void axis()
{

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    /* glBegin(GL_LINES);
     glVertex2i(ortho_max, 0);
     glVertex2i(ortho_min, 0);
     glEnd();
     glBegin(GL_LINES);
     glVertex2i(0,ortho_max);
     glVertex2i(0,ortho_min);
     glEnd();*/
    //glFlush();

    sh_clipping obj;
    obj.set_data();
    obj.show_data();
    obj.draw_clipper();
    obj.draw_poly();
    obj.suthod();
}

void reshape(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width >= height)
    {

        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    }
    else
    {

        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("420 project");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    initGL();
    glutMainLoop();
    return 0;
}
