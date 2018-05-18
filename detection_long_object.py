import numpy as np
import argparse
import cv2
#from PIL import Image
#import PIL.imageOps

cond_quit = 1048689 #valeur correspondant a 'q' pour quitter le programme

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image")
args = vars(ap.parse_args())

# Choix du peripherique de capture. 0 pour /dev/video0.
cap = cv2.VideoCapture(0)


# definition des bornes pour le choix des couleurs
boundaries = [
	([0, 0, 80], [80, 48, 255]) #rouge
	#([0, 0, 80], [100, 48, 255])
	#([0, 60, 0], [70, 255, 60]) #vert
	#([95, 0, 0], [255, 100, 50]) #bleu
]


while True:
 
    # On capture une image
    ret,im = cap.read()
    
    # parcourt les differentes couleurs
    for (lower, upper) in boundaries:
        # create NumPy arrays from the boundaries
        lower = np.array(lower, dtype = "uint8")
        upper = np.array(upper, dtype = "uint8")
 
	    # find the colors within the specified boundaries and apply
	    # the mask
        mask = cv2.inRange(im, lower, upper)
        output = cv2.bitwise_and(im, im, mask = mask)
 
        gray = cv2.cvtColor(output,cv2.COLOR_BGR2GRAY)
        
        gray = np.invert(gray)
        cv2.imshow('gray',gray)
        
        # Setup SimpleBlobDetector parameters.
        params = cv2.SimpleBlobDetector_Params()

        # Change thresholds
        params.minThreshold = 100
        params.maxThreshold = 300


        # Filter by Area.
        params.filterByArea = True
        params.minArea = 300
        params.maxArea = 800

        # Filter by Circularity
        params.filterByCircularity = False
        params.minCircularity = 0.1
        params.maxCircularity = 0.6

        # Filter by Convexity
        params.filterByConvexity = False
        params.minConvexity = 0.87

        # Filter by Inertia
        params.filterByInertia = True
        params.minInertiaRatio = 0.01
        params.maxInertiaRatio = 0.1

        # Create a detector with the parameters
        ver = (cv2.__version__).split('.')
        if int(ver[0]) < 3 :
            detector = cv2.SimpleBlobDetector(params)
        else : 
            detector = cv2.SimpleBlobDetector_create(params)


        # Detect blobs.
        keypoints = detector.detect(gray)

# Draw detected blobs as red circles.
# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures
# the size of the circle corresponds to the size of blob

        im_with_keypoints = cv2.drawKeypoints(output, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# Show blobs
        cv2.imshow("Keypoints", im_with_keypoints)
 
 
        #cv2.circle(output,(300,300),10,(0,255,0))
        #print(output[300][300])
	    # show image
        cv2.imshow("images", np.hstack([im, output]))

        #for i in range(0,len(output[0])):
		#    for j in range(0,len(output)):
		#	    print(output[j][i])
        
        #gray = cv2.cvtColor(output,cv2.COLOR_BGR2GRAY)
        #print(output)
        #edges = cv2.Canny(gray,100,200,apertureSize = 3)
        #cv2.imshow('edges',edges)


        key = cv2.waitKey(10)
    
    # taper 'q' pour quitter le programme
        if key == cond_quit:
            break

