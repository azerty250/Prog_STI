import numpy as np
import argparse
import cv2

cond_quit = 1048689 #valeur correspondant a 'q' pour quitter le programme

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image")
args = vars(ap.parse_args())

# Choix du peripherique de capture. 0 pour /dev/video0.
cap = cv2.VideoCapture(0)


# definition des bornes pour le choix des couleurs
boundaries = [
	([0, 0, 100], [140, 60, 255]), #rouge
	([0, 50, 0], [70, 255, 70]), #vert
	([100, 0, 0], [255, 80, 120]) #bleu
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
 
	# show image
	cv2.imshow("images", np.hstack([im, output]))
	key = cv2.waitKey(10)

    # taper 'q' pour quitter le programme
    if key == cond_quit:
        break

