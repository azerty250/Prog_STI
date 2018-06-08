# import the necessary packages
import numpy as np
import argparse
import cv2
import picamera
import io


center_x = 325 # len(image[0])/2
center_y = 211 #len(image)/2


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
	
	x_pos = 1-x_pos
	y_pos = 1-y_pos
	
	theta = -theta+np.pi/2 - 0.1
	
	return (x_pos,y_pos,theta)

#fonction masquant l'image pour obtenir uniquement le cercle avec les beacons
def masque(im_input):
	#cv2.circle(image, (309,198), 95, (0,0,0), -1)
	
	msq = np.zeros((len(image),len(image[0])),np.uint8)
	
	#cv2.circle(msq, (309,198), 115, (255,255,255), -1)
	#cv2.circle(msq, (309,198), 5, (0,0,0), -1)
	cv2.circle(msq, (center_x,center_y), 135, (255,255,255), -1)
	cv2.circle(msq, (center_x,center_y), 95, (0,0,0), -1)
	
	im_output = cv2.bitwise_and(im_input, im_input, mask = msq)
	#cv2.circle(im_output, (center_x,center_y), 5, (255,0,255), -1)
	cv2.imshow("masquage", im_output)
	return im_output

#masquage dependant de l'angle precedemment calcule
def masque_select(im_input, alpha):
	alpha = -alpha*360/(2*np.pi)+180
	msq = np.zeros((len(image),len(image[0])),np.uint8)
	cv2.ellipse(msq,(center_x,center_y),(135,135),alpha,-255,75,(255,255,255),-1)
	msq = 255-msq
	cv2.imshow("msq",msq)
	im_output = cv2.bitwise_and(im_input, im_input, mask = msq)
	return im_output

def erosion(im_input):
	kernel = np.ones((3,3),np.uint8)
	erosion = cv2.erode(output,kernel,iterations = 1)
	#cv2.imshow("erosion",erosion)
	
	#dilation = cv2.dilate(erosion,kernel,iterations = 1)
	#cv2.imshow("dilation", dilation)
	return erosion

#function that choose the beacons to use
def choix_beacons(angles):
	#i_min = 0
	#for i in range(1,4):
#		if angles[i] < angles[i_min]:
#			i_min = i
#		
#	angle_max = i_min
#	for k in range(1,4)
#		if angles[(i_min+k)%3] > angle_max:
#			angle_max = angles[(i_min+k)%3]
#	print(i_min)
	
	pair = np.zeros((4,1))
	
	#calcul des angles des pairs
	if angles[1] > angles[0]:
		pair[0] = angles[1] - angles[0]
	else:
		pair[0] = 2*np.pi+angle[1] - angle[0]
		
	if angles[2] > angles[1]:
		pair[1] = angles[2] - angles[1]
	else:
		pair[1] = 2*np.pi+angle[2] - angle[1]
		
	if angles[3] > angles[2]:
		pair[2] = angles[3] - angles[2]
	else:
		pair[2] = 2*np.pi+angle[3] - angle[2]
		
	if angles[0] > angles[3]:
		pair[3] = angles[0] - angles[3]
	else:
		pair[3] = 2*np.pi+angle[0] - angle[3]
		
	#print("pairs:")
	#print(pair)
	#recherche de l'angle le plus grand dans les pairs
	pair_max = 0
	for i in range(1,4):
		if(pair[i] > pair[pair_max]):
			pair_max = i
	
	pair_used = (pair_max+2)%4
	
	print("hello")
	print(pair_used)
	if pair[(pair_used+1)%4] > pair[(pair_used+3)%4]:
		unused_beacon = (pair_used+2)%4
	else:
		unused_beacon = (pair_used+3)%4
	
	print(unused_beacon)
	return unused_beacon
	
#fonction calculant la moyenne des positions trouvees
def average(angle):
	position_x = np.zeros((4,1))
	position_y = np.zeros((4,1))
	theta = np.zeros((4,1))

	position_x[0], position_y[0], theta[0] = ToTal_algorithm((0,0),(1,0),(1,1),angle[3],angle[2],angle[1])
	position_x[1], position_y[1], theta[1] = ToTal_algorithm((0,1),(0,0),(1,0),angle[0],angle[3],angle[2])
	position_x[2], position_y[2], theta[2] = ToTal_algorithm((1,1),(0,1),(0,0),angle[1],angle[0],angle[3])
	position_x[3], position_y[3], theta[3] = ToTal_algorithm((1,0),(1,1),(0,1),angle[2],angle[1],angle[0])
	
	average_x = (position_x[0] + position_x[1] + position_x[2] + position_x[3])/4
	average_y = (position_y[0] + position_y[1] + position_y[2] + position_y[3])/4
	average_theta = (theta[0] + theta[1] + theta[2] + theta[3])/4
	
	return (average_x, average_y, average_theta)
	
	
#functions that returs in which part of the arena we are
def quadrant(position_x, position_y):
	if position_x < 0.5:
		if position_y < 0.5:
			quadrant = 3
		else:
			quadrant = 0
	else:
		if position_y < 0.5:
			quadrant = 2
		else:
			quadrant = 1
	return quadrant
	
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

# Choix du peripherique de capture. 0 pour /dev/video0.
#cap = cv2.VideoCapture(0)

camera = picamera.PiCamera()
camera.resolution = (640,480)
#camera.framerate = 2
camera.brightness = 70

stream = io.BytesIO()
camera.capture(stream,format='jpeg')
data = np.fromstring(stream.getvalue(), dtype=np.uint8)
image = cv2.imdecode(data, 1)
cv2.imshow("inutile", image)

angle = np.zeros((4,1))

angle[0] = 3.9
angle[1] = -1.4
angle[2] = 0.4
angle[3] = 2.9

quad = 2


# definition des bornes pour le choix des couleurs
boundaries = [
	([160, 10, 80], [10, 250, 255], 0), #rouge
	([70,50,90],[100,255,255], 1), #vert
	([95, 60, 60], [125, 255, 255], 2), #bleu
	([10, 0, 150], [30, 150, 255], 3) #jaune
]


while True:
	print("passage")
	
	# On capture une image
	#ret,image = cap.read()
	#stream = io.BytesIO()
	camera.brightness = 40
	stream = io.BytesIO()
	camera.capture(stream,format='jpeg')
	data = np.fromstring(stream.getvalue(), dtype=np.uint8)
	image = cv2.imdecode(data, 1)
	cv2.imshow("entree",image)
	
	camera.brightness = 15
	stream = io.BytesIO()
	camera.capture(stream,format='jpeg')
	data = np.fromstring(stream.getvalue(), dtype=np.uint8)
	image_sombre = cv2.imdecode(data, 1)
	cv2.imshow("sombre", image_sombre)
	
	
	image_msq = masque(image)
	image_msq_sombre = masque(image_sombre)

	image_hsv = cv2.cvtColor(image_msq,  cv2.COLOR_BGR2HSV)
	image_hsv_sombre = cv2.cvtColor(image_msq_sombre,  cv2.COLOR_BGR2HSV)


	# loop over the boundaries
	for (lower, upper, num) in boundaries:
	
		# create NumPy arrays from the boundaries
		lower = np.array(lower, dtype = "uint8")
		upper = np.array(upper, dtype = "uint8")
		
		if angle[num] != 0:
			#if angle[num] == quad:
			#	image_hsv_passage = masque_select(image_hsv_sombre,angle[num])
			#else:
			image_hsv_passage = masque_select(image_hsv,angle[num])
		else:
			#if angle[num] == quad:
			#	image_hsv_passage = image_hsv_sombre
			#else:
			image_hsv_passage = image_hsv
		
		masquee = cv2.cvtColor(image_hsv_passage, cv2.COLOR_HSV2BGR)
		cv2.imshow("masque", masquee)
		
		if lower[0] > upper[0]:
			# find the colors within the specified boundaries and apply
			# the mask
			mask1 = cv2.inRange(image_hsv_passage, np.array([0, lower[1], lower[2]], dtype = "uint8"), upper)
			mask2 = cv2.inRange(image_hsv_passage, lower, np.array([255, upper[1], upper[2]], dtype = "uint8"))
			mask = mask1 | mask2
		else:
			mask = cv2.inRange(image_hsv_passage, lower, upper)
		
		output_hsv = cv2.bitwise_and(image_hsv_passage, image_hsv_passage, mask = mask)

		output = cv2.cvtColor(output_hsv, cv2.COLOR_HSV2BGR)

		cv2.imshow("couleurs", output)

		if num == 0 or num == 1:# or num == 3:
			output = erosion(output)
		
		y, x, _ = np.nonzero(output)
		if(len(x) != 0 and len(y) != 0):
			x_beacon = (x[0]+x[len(x)-1])/2
			y_beacon = (y[0]+y[len(y)-1])/2
			tan = float(-(x_beacon-center_x))/float(-(y_beacon-center_y))
			if -(y[0]-center_y) > 0:
				angle[num] = np.arctan(tan)
			else:
				angle[num] = np.arctan(tan)+np.pi
	
		for a in angle:
			cv2.line(image, (center_x,center_y), (center_x-100*np.cos(a-np.pi/2),center_y + 100*np.sin(a-np.pi/2)),(0,0,255))
		
		# show the images
		cv2.imshow("images", np.hstack([image, output]))

		print(angle[num])
		
		
		cv2.waitKey(0)
		


	#compute the robot position and orientation using ToTal algorithm
#	if angle[0] != 0 and angle[1] != 0 and angle[2] != 0 and angle[3] != 0:
#		if choix_beacons(angle) == 0:
#			position_x, position_y, theta = ToTal_algorithm((0,0),(1,0),(1,1),angle[3],angle[2],angle[1])
#			print("on utilise pas le rouge")
#	
#		if choix_beacons(angle) == 1:
#			position_x, position_y, theta = ToTal_algorithm((0,1),(0,0),(1,0),angle[0],angle[3],angle[2])
#			print("on utilise pas le vert")
#			
#		if choix_beacons(angle) == 2:
#			position_x, position_y, theta = ToTal_algorithm((1,1),(0,1),(0,0),angle[1],angle[0],angle[3])
#			print("on utilise pas le bleu")
#			
#		if choix_beacons(angle) == 3:
#			position_x, position_y, theta = ToTal_algorithm((1,0),(1,1),(0,1),angle[2],angle[1],angle[0])
#			print("on utilise pas le jaune")
#	else:
#		position_x, position_y, theta = ToTal_algorithm((0,1),(1,1),(1,0),angle[0],angle[1],angle[2])

	#calcul du quadrant dans lequel le robot se trouve
#	quad = quadrant(position_x, position_y)


	position_x, position_y, theta = average(angle)
	
	#position_ref_x, position_ref_y, theta_ref = ToTal_algorithm((0,1),(1,1),(1,0),3.9270,-0.78540,0.78540)
	#print(position_ref_x*8)
	#print(position_ref_y*8)
	#print(theta_ref)
	#print(angle)
	position_x = position_x *8
	position_y = position_y *8
	theta = theta * 360 /(2*np.pi)
	
	print(theta)
	print(position_x)
	print(position_y)
	#cv2.waitKey(0)
	
	

	#dessin(position_x, position_y, theta)
	
	#cv2.waitKey(0)
