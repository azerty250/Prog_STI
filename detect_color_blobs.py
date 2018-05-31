# import the necessary packages
import numpy as np
import argparse
import cv2
 
# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image")
args = vars(ap.parse_args())
 
# load the image
image = cv2.imread('beacon_exemple.png')

image_hsv = cv2.cvtColor(image,  cv2.COLOR_BGR2HSV)

# definition des bornes pour le choix des couleurs
boundaries = [
	([0, 100, 100], [20, 255, 255]), #rouge
	([80,100,100],[90,255,255]), #vert
	([100, 100, 100], [120, 255, 255]), #bleu
	([0, 0, 150], [50, 100, 255]) #jaune
]

i = 0

# loop over the boundaries
for (lower, upper) in boundaries:
	# create NumPy arrays from the boundaries
	lower = np.array(lower, dtype = "uint8")
	upper = np.array(upper, dtype = "uint8")
 
	# find the colors within the specified boundaries and apply
	# the mask
	mask = cv2.inRange(image_hsv, lower, upper)
	
	output_hsv = cv2.bitwise_and(image_hsv, image_hsv, mask = mask)

	output = cv2.cvtColor(output_hsv, cv2.COLOR_HSV2BGR)
	# show the images
	cv2.imshow("images", np.hstack([image, output]))
	
	output2 = 255-output
	cv2.imshow("output2", output2)

	# Setup SimpleBlobDetector parameters.
	params = cv2.SimpleBlobDetector_Params()

	# Change thresholds
	params.minThreshold = 1
	params.maxThreshold = 1000


	# Filter by Area.
	params.filterByArea = False
	params.minArea = 1000

	# Filter by Circularity
	params.filterByCircularity = False
	params.minCircularity = 0.1

	# Filter by Convexity
	params.filterByConvexity = False
	params.minConvexity = 0.87
    
	# Filter by Inertia
	params.filterByInertia = False
	params.minInertiaRatio = 0.01
	params.maxInertiaRatio = 0.5

	# Create a detector with the parameters
	ver = (cv2.__version__).split('.')
	if int(ver[0]) < 3 :
		detector = cv2.SimpleBlobDetector(params)
	else : 
		detector = cv2.SimpleBlobDetector_create(params)


	# Detect blobs.
	keypoints = detector.detect(output2)

	# Draw detected blobs as red circles.
	# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures
	# the size of the circle corresponds to the size of blob

	im_with_keypoints = cv2.drawKeypoints(output2, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

	# Show blobs
	cv2.imshow("Keypoints", im_with_keypoints)
	i = i+1
	cv2.waitKey(0)
