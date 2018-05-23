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
	
	#compute the robot orientation
	if y1-y_pos > 0:
		theta = np.arctan(-(x1-x_pos)/(y1-y_pos))-phi1
	else:
		theta = np.arctan(-(x1-x_pos)/(y1-y_pos))-phi1+np.pi
	
	return (x_pos,y_pos,theta)

def dessin(position_x, position_y, theta):
	dessin = np.zeros((1020,1020))
	dessin = dessin + 255

	cv2.rectangle(dessin,(10,10),(1010,1010),(0,0,0))

	cv2.circle(dessin,(int(1000*position_x),int(1000*position_y)),10,(0,0,0))
	cv2.arrowedLine(dessin,(1000*position_x,1000*position_y),(int(1000*position_x-50*np.sin(theta)),int(1000*position_y-50*np.cos(theta))),(0,255,0))

	cv2.imshow('dessin',dessin)

	cv2.waitKey(0)



# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image")
args = vars(ap.parse_args())

# load the image
image = cv2.imread('beacon_exemple_retouches.png')

center_x = len(image[0])/2
center_y = len(image)/2


# definition des bornes pour le choix des couleurs
boundaries = [
	([0, 0, 240], [50, 50, 255], 0), #rouge
	([0, 200, 200], [50, 255, 255], 1), #jaune
	([180, 0, 0], [255, 70, 70], 2), #bleu
	([0, 240, 0], [50, 255, 50], 3) #vert
]

angle = np.zeros((4,1))

# loop over the boundaries
for (lower, upper, num) in boundaries:
	# create NumPy arrays from the boundaries
	lower = np.array(lower, dtype = "uint8")
	upper = np.array(upper, dtype = "uint8")
 
	# find the colors within the specified boundaries and apply
	# the mask
	mask = cv2.inRange(image, lower, upper)
	output = cv2.bitwise_and(image, image, mask = mask)

	y, x, _ = np.nonzero(output)
	tan = float(-(x[0]-center_x))/float(-(y[0]-center_y))
	if -(y[0]-center_y) > 0:
		angle[num] = np.arctan(tan)
	else:
		angle[num] = np.arctan(tan)+np.pi
	
	# show the images
	cv2.imshow("images", np.hstack([image, output]))
	cv2.waitKey(0)


#compute the robot position and orientation using ToTal algorithm
position_x, position_y, theta = ToTal_algorithm((0,1),(0,0),(1,0),angle[0],angle[1],angle[2])
print(angle)
print(theta)
print(position_x)
print(position_y)

dessin(position_x, position_y, theta)
