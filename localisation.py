# import the necessary packages
import numpy as np
import argparse
import cv2


def ToTal_algorithm((x1,y1),(x2,y2),(x3,y3),phi1,phi2,phi3):
	
	#compute the modified beacon coordinates
	x1_p = x1-x2
	x3_p = x3-x2
	y1_p = y1-y2
	y3_p = y3-y2
	
	#compute the cot
	T12 = 1/np.tan(phi2-phi1)
	T23 = 1/np.tan(phi3-phi2)
	T31 = (1-T12*T23)/(T12+T23)
	
	#compute the modified circle center coordinates
	x12_p = x1_p + T12*y1_p
	x23_p = x3_p - T23*y3_p
	x31_p = x3_p + x1_p + T31*(y3_p-y1_p)
	y12_p = y1_p - T12*x1_p
	y23_p = y3_p + T23*x3_p
	y31_p = y3_p + y1_p - T31*(x3_p-x1_p)
	
	k31_p = x1_p*x3_p + y1_p*y3_p + T31*(x1_p*y3_p - x3_p*y1_p)
	
	D = (x12_p-x23_p)*(y23_p-y31_p) - (y12_p-y23_p)*(x23_p-x31_p)
	
	#compute the robot position
	x_pos = x2 + (k31_p*(y12_p-y23_p))/D
	y_pos = y2 + (k31_p*(x23_p-x12_p))/D
	
	return (x_pos,y_pos)


dessin = np.zeros((1020,1020))
dessin = dessin + 255
#print(dessin)

cv2.rectangle(dessin,(10,10),(1010,1010),(0,0,0))
position_x, position_y = ToTal_algorithm((0,1),(0,0),(1,0),-np.pi/2,0,np.pi/2)

#position_x = -1/(tan_gamma-tan_delta)
#position_y = -position_x * tan_gamma
print(position_x)
print(position_y)

cv2.circle(dessin,(int(1000*position_x),int(1000*position_y)),10,(0,0,0))

cv2.imshow('dessin',dessin)

cv2.waitKey(0)

