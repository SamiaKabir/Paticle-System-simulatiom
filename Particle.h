//
//  Particle.h
//  Homework 2_Particle system
//
//  Created by Samia Kabir on 9/24/19.
//  Copyright © 2019 Samia Kabir. All rights reserved.
//

#ifndef Particle_h
#define Particle_h
#include "Eigen/Dense"
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#include <random>
#include <vector>


using namespace Eigen;
using namespace std;

Vector3d Vnew,Xnew;
Vector3d W(6,1,-3);
Vector3d F,F_air, F_g, F_wind,F_p;
Vector3d g(0,-10,0);
double d= 0.005;  //air resistance
double timestep,timestep_remain=0.50,t_max=0.50;
double distance_n;
double distance_new;
Vector3d plane_normal(0,1,0);
Vector3d plane_point(-100, 0, 90);
Vector3d x_1(0, 25, 0),y_1(-30, 0, 30),z_1(30, 0, 30);
Vector3d x_2(30, 0, 30),y_2(0, 0, -30),z_2(0, 25, 0);
Vector3d x_3(0, 0, -30),y_3(0, 25, 0),z_3(-30, 0, 30);
Vector3d p_1(0, 25, 0), p_2(30, 0, 30), p_3(0, 0, -30);
Vector3d plane_normal_1, plane_normal_2, plane_normal_3;
double f;

Vector3d unit1,unit2,unit3,unit4;
Vector3d unit5,unit6,unit7,unit8,unit9,unit10,unit11,unit12,unit13;
Vector3d a(-100, -10, 90), b(100, -10, 90),e(100, -10, -80),c(-100, -10, -80);

double kappa_a=0.4;  //   importance of potential field
double epsilon=0.15;  //for collision and fixing the bounce back
Vector3d D;
bool heat_up=false;

//random wind generation
unsigned seed0 = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator_2(seed0);
uniform_real_distribution<double> distribution_x(-7,7);
uniform_real_distribution<double> distribution_y(0,3);
uniform_real_distribution<double> distribution_z(-7,7);

vector<Vector3d> potential_field;
vector<double>   field_value;


void generate_potential_field(){
    
    int tv;
    for(int i=0;i<24;i++){
        Vector3d temp;
        potential_field.push_back(temp);
    }
    
    tv=0;
    for(int i=75;i>=0;i-=25){
        for(int j=tv;j<24;j+=4){
           potential_field.at(j)[0]=i;
        }
        tv++;
    }
    tv=0;
    for(int i=50;i>=25;i-=25){

        for(int j=tv;j<12;j++){
            potential_field[j][1]=i;

        }
        tv=12;
        for(int j=tv;j<24;j++){
            potential_field[j][1]=i;
            
        }
        
    }
    tv=0;
    for(int i=-20;i>=-70;i-=25){
        int j;
        for(j=tv;j<(4+tv);j++){
            potential_field[j][2]=i;
            potential_field[j+12][2]=i;
        }
        tv=j;
    }
    
    for(int i=0;i<24;i++){
        
        field_value.push_back(0);
        
    }
    
//    
//    for(int i=0;i<24;i++){
//      printf("%f %f %f \n", potential_field[i][0],potential_field[i][1], potential_field[i][2] );
//    }
    //remove the comment to see the effect of potential field
//     field_value[9]=2;   field_value[10]=4;
//     field_value[12]=4;   field_value[13]=2;
//     field_value[0]=2;   field_value[1]=2;

}


Vector3d calculate_del_P(Vector3d pos){
    
    Vector3d P;
    
    for(int i=0; i<24; i++){
        
        if(pos[0]-potential_field.at(i)[0]<=25 &&
           pos[1]-potential_field.at(i)[1]<=25 &&
           pos[2]-potential_field.at(i)[2]<=25)
        {
            if(i==0){
                P[0]=field_value.at(i+1);
                P[1]=field_value.at(i+1);
                P[2]=field_value.at(i+1);
                
            }
            else if(i==23){
                P[0]=field_value.at(i-1);
                P[1]=field_value.at(i-1);
                P[2]=field_value.at(i-1);
            }
            
            else{
                P[0]=(field_value.at(i-1)+field_value.at(i+1))/2;
                P[1]=(field_value.at(i-1)+field_value.at(i+1))/2;
                P[2]=(field_value.at(i-1)+field_value.at(i+1))/2;
            }
            //break;
        }
    }
    return P;
}



void set_heat(){
    
    heat_up= !(heat_up);
}


void wind_value(int val)
{
    if(val==0 && W[0]>=0){
        W[0]=W[0]*(-1);
    }
    
    else if(val==1 && W[0]<=0){
        W[0]=W[0]*(-1);
    }
    
    else if(val==2 && W[2]>=0){
        W[2]=W[2]*(-1);
    }
    
    else if(val==3 && W[2]<=0){
        W[2]=W[2]*(-1);
    }
    
    else if(val==4){
        
        W[0]=distribution_x(generator_2);
        W[1]=distribution_y(generator_2);
        W[2]=distribution_z(generator_2);
    }
    
    else;
}

class Particle{
 
public:
    Vector3d position;
    Vector3d velocity;
    Vector3d acceleration;
    double age;
    double mass;
    double radius;
    bool isdead;

    
    
    Particle(){
        radius=0.25;
        age=0;
        mass=0.009;
        isdead=false;
        acceleration[0]=0;acceleration[1]=-10; acceleration[2]=0;
        velocity[0]=0; velocity[1]=0; velocity[2]=0;
    }
    
    void collision();
    void draw(GLUquadricObj *sphere,GLuint texture_snow);
 
};

void Particle::draw(GLUquadricObj *sphere,GLuint texture_snow){
    
    if(!isdead){
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);
        glTexCoord2f(0.0, 0.0);
        glBindTexture(GL_TEXTURE_2D, texture_snow);
        gluSphere(sphere, this->radius, 10, 10);
        glDisable(GL_BLEND);
        glPopMatrix();
    }
    
    else if(isdead && heat_up && age>50)
    {
        
        glPushMatrix();
        glTexCoord2f(0.0, 0.0);
        glTranslatef(position[0], position[1], position[2]);
        glColor4f(1.0, 1.0, 1.0, 0.9);
        glBegin(GL_QUADS);
        glVertex3f(-radius,radius,radius);
        glVertex3f(radius, radius, radius);
        glVertex3f(radius, radius, -radius);
        glVertex3f(-radius, radius, -radius);
        glEnd();
        glPopMatrix();
        
    }
    
    else{
        glPushMatrix();
        glTranslatef(position[0], position[1], position[2]);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);
        glTexCoord2f(0.0, 0.0);
        glBindTexture(GL_TEXTURE_2D, texture_snow);
        gluSphere(sphere, this->radius, 10, 10);
        glDisable(GL_BLEND);
        glPopMatrix();
    }
    
    
}

void Particle:: collision(){
    
    for(double i=0;i<t_max;i+=timestep_remain){
        ///////////////////////////// find acceleration////////////////////
        if(!isdead){
            F_air= -d*(velocity);
            F_g= mass*g;
            F_wind= d*(W);
            F_p= (-1) * kappa_a*calculate_del_P(position); //force due to potential field
            F= F_air+F_g+F_wind+F_p;

            acceleration= F/mass;
            
            
            ///////////////////////////// find Xnew, Vnew////////////////////
            
            Vnew= velocity+timestep_remain*acceleration;
            Xnew= position+ timestep_remain*velocity;
        }
        //collision detection
        
        if(!isdead){
           //collision with 1st triangle
            plane_normal_1= (z_1-y_1).cross(x_1-y_1);
        
            distance_n= (position-p_1).dot(plane_normal_1);
            distance_new=(Xnew-p_1).dot(plane_normal_1);
            if(distance_n * distance_new <0){
                unit5= (z_1-y_1).cross(Xnew-y_1);
                unit6= (x_1-z_1).cross(Xnew-z_1);
                unit7= (y_1-x_1).cross(Xnew-x_1);
                
                if((unit5.dot(plane_normal_1) >0 && unit6.dot(plane_normal_1) >0 && unit7.dot(plane_normal_1) >0) )
                {
                    f= distance_n/ (distance_n-distance_new);
                    Xnew= position+ 1.1*f*timestep_remain*velocity;
                    Vnew[0]= 0; Vnew[1]=0; Vnew[2]=0;
                    velocity= Vnew;
                    position= Xnew;
                    isdead=true;
                    continue;
                }
                else{
                    velocity= Vnew;
                    position= Xnew;
                }
            }
            //collision with 2nd triangle
            else {
                plane_normal_2= (y_2-x_2).cross(z_2-x_2);
                distance_n= (position-p_2).dot(plane_normal_2);
                distance_new=(Xnew-p_2).dot(plane_normal_2);
                if(distance_n * distance_new <0){
                    unit8= (y_2-x_2).cross(Xnew-x_2);
                    unit9= (z_2-y_2).cross(Xnew-y_2);
                    unit10=(x_2-z_2).cross(Xnew-z_2);
                    
                    if((unit8.dot(plane_normal_2) >0 && unit9.dot(plane_normal_2) >0 && unit10.dot(plane_normal_2) >0) )
                    {
                        f= distance_n/ (distance_n-distance_new);
                        Xnew= position+ f*timestep_remain*velocity;
                        Vnew[0]= 0; Vnew[1]=0; Vnew[2]=0;
                        velocity= Vnew;
                        position= Xnew;
                        isdead=true;
                        continue;
                    }
                    else{
                        velocity= Vnew;
                        position= Xnew;
                    }
                }
                //collision with 3rd triangle
                else{
                    
                    plane_normal_3 =(x_3-z_3).cross(y_3-z_3);
                    distance_n= (position-p_3).dot(plane_normal_3);
                    distance_new=(Xnew-p_3).dot(plane_normal_3);
                    if(distance_n * distance_new <0){
                        unit11= (y_3-x_3).cross(Xnew-x_3);
                        unit12= (z_3-y_3).cross(Xnew-y_3);
                        unit13= (x_3-z_3).cross(Xnew-z_3);
                        
                        if((unit11.dot(plane_normal_3) >0 && unit12.dot(plane_normal_3) >0 && unit13.dot(plane_normal_3) >0) )
                        {
                            
                            f= distance_n/ (distance_n-distance_new);
                            Xnew= position+ f*timestep_remain*velocity;
                            Vnew[0]= 0; Vnew[1]=0; Vnew[2]=0;
                            velocity= Vnew;
                            position= Xnew;
                            isdead=true;
                            continue;
                            
                        }
                        else{
                            velocity= Vnew;
                            position= Xnew;
                            
                        }
                     }
                    
                    else{
                        //collision with the polygon base
                        distance_n= (position-plane_point).dot(plane_normal);
                        distance_new=(Xnew-plane_point).dot(plane_normal);
                        if(distance_n * distance_new <0)
                        {
                            
                            unit1= (b-a).cross(Xnew-a);
                            unit2= (e-b).cross(Xnew-b);
                            unit3= (c-e). cross (Xnew-e);
                            unit4= (a-c). cross(Xnew-c);
                            
                            
                            if(unit1.dot(plane_normal) >=0 &&
                               unit2.dot(plane_normal) >=0 &&
                               unit3.dot(plane_normal) >=0 &&
                               unit4.dot(plane_normal) >=0 )
                                
                            {
                                D= ((Xnew-plane_point).dot(plane_normal))* plane_normal;
                                Xnew-= (1+epsilon)* D;
                                Vnew[0]= 0; Vnew[1]=0; Vnew[2]=0;
                                velocity= Vnew;
                                position= Xnew;
                                isdead=true;
                                continue;
                            }
                            
                            else{
                                velocity= Vnew;
                                position= Xnew;
                            }
                            
                        }
                        
                        else{
                            velocity= Vnew;
                            position= Xnew;
                        }
                     }
                
                  }
              }
        }
        
 

    }
    
    radius+=0.0005;
    mass+=0.00001;
    age++;
    
}


#endif /* Particle_h */
