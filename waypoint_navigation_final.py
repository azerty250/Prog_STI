# import the necessary packages
import numpy as np
import argparse
import cv2
import picamera
import io
import time
import serial

deux_pi = np.pi+np.pi

center_x = 325
center_y = 211

kernel_rouge = np.ones((2,2),np.uint8)
kernel_vert = np.ones((3,3),np.uint8)
kernel_jaune = np.ones((2,3),np.uint8)
kernel_bleu = np.ones((1,2),np.uint8)

waypoint_array = [(6,4)]

ser = serial.Serial('/dev/ttyACM0', 57600)


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
		theta = np.arctan((x_pos-x1)/(y1-y_pos))-phi1
	else:
		theta = np.arctan((x_pos-x1)/(y1-y_pos))-phi1+np.pi

	if theta < 0:
		theta = theta+deux_pi
	else:
		if theta > deux_pi:
			theta = theta-deux_pi
	
	return (x_pos,y_pos,theta)

#fonction masquant l'image pour obtenir uniquement le cercle avec les beacons
def masque(im_input):
	
	msq = np.zeros((len(image),len(image[0])),np.uint8)
	
	cv2.circle(msq, (center_x,center_y), 135, (255,255,255), -1)
	cv2.circle(msq, (center_x,center_y), 95, (0,0,0), -1)
	
	im_output = cv2.bitwise_and(im_input, im_input, mask = msq)

	return im_output

#masquage dependant de l'angle precedemment calcule
def masque_select(im_input, alpha):
	alpha = -alpha*180/np.pi+180
	msq = np.zeros((len(image),len(image[0])),np.uint8)
	cv2.ellipse(msq,(center_x,center_y),(135,135),alpha,-230,50,(255,255,255),-1)
	msq = 255-msq

	im_output = cv2.bitwise_and(im_input, im_input, mask = msq)
	return im_output


#fonction calculant la moyenne des positions trouvees
def average(angle):
	position_x = np.zeros((4,1))
	position_y = np.zeros((4,1))
	theta = np.zeros((4,1))

	position_x[0], position_y[0], theta[0] = ToTal_algorithm((0,0),(1,0),(1,1),-angle[3],-angle[2],-angle[1])
	position_x[1], position_y[1], theta[1] = ToTal_algorithm((0,1),(0,0),(1,0),-angle[0],-angle[3],-angle[2])
	position_x[2], position_y[2], theta[2] = ToTal_algorithm((1,1),(0,1),(0,0),-angle[1],-angle[0],-angle[3])
	position_x[3], position_y[3], theta[3] = ToTal_algorithm((1,0),(1,1),(0,1),-angle[2],-angle[1],-angle[0])

	
	average_x = (position_x[0] + position_x[1] + position_x[2] + position_x[3])/4
	average_y = (position_y[0] + position_y[1] + position_y[2] + position_y[3])/4
	
	return (average_x, average_y, theta[0])
	
	
def go_to_point(posx_robot, posy_robot, theta_robot, posx_goal, posy_goal, avance, rotation):
	
	
	diff_x = posx_robot-posx_goal
	diff_y = posy_goal-posy_robot
	
	diff_theta = np.arctan(diff_x/diff_y)*180/np.pi-theta_robot
	
	
	if diff_theta < -180:
		diff_theta = diff_theta + 360
	else:
		if diff_theta > 180:
			diff_theta = diff_theta - 360
	
	print("diff_theta:")
	print(diff_theta)
	
	#we begin with the rotation and when the robot has the good orientation
	#then we make the translation
	if diff_theta > 20 and avance == 0:
		commande = 'g'
		rotation = 1
	elif diff_theta < -20 and avance == 0:
		commande = 'd'
		rotation = 1
	elif np.absolute(diff_theta) > 20 and avance == 0:
		commande = '0'
	else:
		commande = 'a'
		avance = 1
	
	return (commande, avance, rotation)

def send_arduino(commande):
	message = "{0}".format(commande)
	ser.write(message)

	print(message)



# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path topy the image")
args = vars(ap.parse_args())

camera = picamera.PiCamera()
camera.resolution = (640,480)
camera.brightness = 40

stream = io.BytesIO()
camera.capture(stream,format='jpeg')
data = np.fromstring(stream.getvalue(), dtype=np.uint8)
image = cv2.imdecode(data, 1)
cv2.imshow("inutile", image)
angle = np.zeros((4,1))

quad = 2
levage = 0
rotation = 0

compteur_points = 0
pente_levee = 0
avance = 0


# definition des bornes pour le choix des couleurs
boundaries = [
	([160, 10, 80], [10, 250, 255], 0), #rouge
	([70,50,90],[100,255,255], 1), #vert
	([95, 60, 60], [125, 255, 255], 2), #bleu
	([10, 0, 150], [30, 150, 255], 3) #jaune
]


while True:
	print("passage")
	print(time.clock())
	
	# On capture une image
	stream = io.BytesIO()
	camera.capture(stream,format='jpeg')
	data = np.fromstring(stream.getvalue(), dtype=np.uint8)
	image = cv2.imdecode(data, 1)
	
	image_msq = masque(image)
	image_hsv = cv2.cvtColor(image_msq,  cv2.COLOR_BGR2HSV)

	
	# loop over the boundaries
	for (lower, upper, num) in boundaries:
		# create NumPy arrays from the boundaries
		lower = np.array(lower, dtype = "uint8")
		upper = np.array(upper, dtype = "uint8")
		
		image_hsv_passage = image_hsv
		
		if lower[0] > upper[0]:
			# find the colors within the specified boundaries and apply
			# the mask
			mask1 = cv2.inRange(image_hsv_passage, np.array([0, lower[1], lower[2]], dtype = "uint8"), upper)
			mask2 = cv2.inRange(image_hsv_passage, lower, np.array([255, upper[1], upper[2]], dtype = "uint8"))
			mask = mask1 | mask2
		else:
			mask = cv2.inRange(image_hsv_passage, lower, upper)
		
		output_hsv = cv2.bitwise_and(image_hsv_passage, image_hsv_passage, mask = mask)

		
		if num == 0:
			output_hsv = cv2.erode(output_hsv,kernel_rouge,iterations = 1)
			
		if num == 1:
			output_hsv = cv2.erode(output_hsv,kernel_vert,iterations = 1)
			
		if num == 3:
			output_hsv = cv2.erode(output_hsv,kernel_jaune,iterations = 1)
		
		if num == 2:
			output_hsv = cv2.erode(output_hsv,kernel_bleu,iterations = 1)

		y, x, _ = np.nonzero(output_hsv)
		if(len(x) != 0 and len(y) != 0):
			x_beacon = (x[0]+x[len(x)-1])/2
			y_beacon = (y[0]+y[len(y)-1])/2
			diff_beacon_y = center_y-y_beacon
			if diff_beacon_y != 0:
				tan = (float)(100*(center_x-x_beacon)/(diff_beacon_y))
			else:
				tan = 10000
			if diff_beacon_y > 0:
				angle[num] = np.arctan(tan/100)
			else:
				angle[num] = np.arctan(tan/100)+np.pi
	
		for a in angle:
			cv2.line(image, (center_x,center_y), (center_x-100*np.cos(a-np.pi/2),center_y + 100*np.sin(a-np.pi/2)),(0,0,255))
		
	position_x, position_y, theta = average(angle)
	
	position_x = position_x *8
	position_y = position_y *8
	theta = theta * 180 / np.pi
	
	print(theta)
	print(position_x)
	print(position_y)
	
	if (position_x > waypoint_array[compteur_points][0] - 0.3 and
	position_x < waypoint_array[compteur_points][0] + 0.3 and
	position_y > waypoint_array[compteur_points][1] - 0.3 and
	position_y < waypoint_array[compteur_points][1] + 0.3):
		compteur_points = compteur_points+1
	
	if(compteur_points == len(waypoint_array)):
		mes = "{0}".format('s')
		ser.write(mes)
		
	print compteur_points
	
	commande, avance, rotation = go_to_point(position_x,position_y,theta, waypoint_array[compteur_points][0],waypoint_array[compteur_points][1], avance, rotation)
	
	print("commande:")
	print(commande)
	send_arduino(commande)

