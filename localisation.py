# import the necessary packages
import numpy as np
import argparse
import cv2


#function computing the position and the orientation of the robot
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

#fonction masquant l'image pour obtenir uniquement le cercle avec les beacons
def masque(im_input):
	#cv2.circle(image, (309,198), 95, (0,0,0), -1)
	
	msq = np.zeros((len(image),len(image[0])),np.uint8)
	
	cv2.circle(msq, (309,198), 115, (255,255,255), -1)
	cv2.circle(msq, (309,198), 95, (0,0,0), -1)
	
	im_output = cv2.bitwise_and(im_input, im_input, mask = msq)
	
	return im_output

#function displaying the result
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
image = cv2.imread('donnees/essais_sombre2.png')

image_msq = masque(image)

image_hsv = cv2.cvtColor(image_msq,  cv2.COLOR_BGR2HSV)

center_x = 309 # len(image[0])/2
center_y = 198 #len(image)/2


# definition des bornes pour le choix des couleurs
boundaries = [
	([170, 150, 100], [40, 255, 255], 0), #rouge
	([70,100,100],[100,255,255], 1), #vert
	([100, 110, 110], [115, 255, 255], 2), #bleu
	([25, 0, 50], [45, 50, 255], 3) #jaune
]


angle = np.zeros((4,1))

# loop over the boundaries
for (lower, upper, num) in boundaries:
	
	# create NumPy arrays from the boundaries
	lower = np.array(lower, dtype = "uint8")
	upper = np.array(upper, dtype = "uint8")
 
	if lower[0] > upper[0]:
		# find the colors within the specified boundaries and apply
		# the mask
		mask1 = cv2.inRange(image_hsv, np.array([0, lower[1], lower[2]], dtype = "uint8"), upper)
		mask2 = cv2.inRange(image_hsv, lower, np.array([255, upper[1], upper[2]], dtype = "uint8"))
		mask = mask1 | mask2
	else:
		mask = cv2.inRange(image_hsv, lower, upper)
	
	output_hsv = cv2.bitwise_and(image_hsv, image_hsv, mask = mask)

	output = cv2.cvtColor(output_hsv, cv2.COLOR_HSV2BGR)

	kernel = np.ones((3,3),np.uint8)
	erosion = cv2.erode(output,kernel,iterations = 1)
	cv2.imshow("erosion",erosion)
	
	dilation = cv2.dilate(erosion,kernel,iterations = 1)
	cv2.imshow("dilation", dilation)
	
	y, x, _ = np.nonzero(dilation)
	tan = float(-(x[0]-center_x))/float(-(y[0]-center_y))
	if -(y[0]-center_y) > 0:
		angle[num] = np.arctan(tan)
	else:
		angle[num] = np.arctan(tan)+np.pi
	
	# show the images
	cv2.imshow("images", np.hstack([image, dilation]))
	cv2.waitKey(0)


#compute the robot position and orientation using ToTal algorithm
position_x, position_y, theta = ToTal_algorithm((0,1),(0,0),(1,0),angle[0],angle[1],angle[2])
#print(angle)
print(theta)
print(position_x)
print(position_y)

dessin(position_x, position_y, theta)
