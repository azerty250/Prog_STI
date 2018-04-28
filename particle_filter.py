

import numpy as np
import time
import sys


##########################
# All useful variables #
##########################

first_call = True

my_particles = []

##########################
# All constant variables #
##########################

NUMBER_PARTICLES = 1000

HEIGHT = 8
WIDTH =  8

STARTING_AREA_MIN = 0
STARTING_AREA_MAX = 1

DEV_XY = 0.2
DEV_THETA = 0.1

##################################
# Definition of useful functions #
##################################

def init_rnd_pos() :

    x = uniform(STARTING_AREA_MIN,STARTING_AREA_MAX)
    y = uniform(STARTING_AREA_MIN,STARTING_AREA_MAX)
    theta = uniform(STARTING_AREA_MIN,STARTING_AREA_MAX) * 2 * np.pi

    return [x,y,theta]



####################################
# Definition of the Particle class #
####################################

class Particle : 

    def __init__(self, position = [0.,0.,0.]) : 

        x,y,theta = position        

    def set_pos(self, x,y, theta) :

        self.x = x
        self.y = y
        self.theta = theta

    def update_pos(self, dx, dy, dtheta):

        self.x = self.x + dx * np.cos(self.theta + dtheta)
        self.y = self.y + dy * np.sin(self.theta + dtheta)
        self.theta = self.theta + dtheta



#####################################################
# Defintion of the functions computing the position # 
#####################################################

def init_all_particles() :

    for i in range(NUMBER_PARTICLES) :

        p = Particle(position = init_rnd_pos())
        my_particles.append(p)


def time_update() :
    #In here we will use the data coming from the encoders to update the state of the particles





def measurement_update() :
    #In here we will update the weight associatied to each particles based on the beacons measurement



def estimated_pos() :
    #In here we will choose the most probable position of the robot


def particle_filter(x_cam, y_cam, theta_cam) :

    if first_call == True : 
        init_all_particles()
        first_call = False
    else
        time_update()
        measurement_update()
        
        return estimated_pos()


