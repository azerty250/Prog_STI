import numpy as np
import argparse
import cv2

import matplotlib.pyplot as plt
import scipy.misc
import skimage.filters

cond_quit = 1048689 #valeur correspondant a 'q' pour quitter le programme

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image")
args = vars(ap.parse_args())

# Choix du peripherique de capture. 0 pour /dev/video0.
cap = cv2.VideoCapture(0)


# definition des bornes pour le choix des couleurs
boundaries = [
	([0, 0, 80], [80, 48, 255]), #rouge
	([0, 60, 0], [70, 255, 60]) #vert
	#([80, 0, 0], [255, 80, 50]) #bleu
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



        #gray = cv2.cvtColor(output,cv2.COLOR_BGR2GRAY)

        #edges = cv2.Canny(gray,100,200,apertureSize = 3)
        #cv2.imshow('edges',edges)

        
        y, x, _ = np.nonzero(output)


        x = x - np.mean(x)
        y = y - np.mean(y)
        coords = np.vstack([x, y])


        print(coords)
        cov = np.cov(coords)
        evals, evecs = np.linalg.eig(cov)

        sort_indices = np.argsort(evals)[::-1]
        x_v1, y_v1 = evecs[:, sort_indices[0]]  # Eigenvector with largest eigenvalue
        x_v2, y_v2 = evecs[:, sort_indices[1]]
        print("x_v1: ",x_v1)
        print("y_v1: ",y_v1)

        scale = 20
        plt.plot([x_v1*-scale*2, x_v1*scale*2],
                 [y_v1*-scale*2, y_v1*scale*2], color='red')
        plt.plot([x_v2*-scale, x_v2*scale],
                 [y_v2*-scale, y_v2*scale], color='blue')
        #print(
        plt.plot(x, y, 'k.')
        plt.axis('equal')
        plt.gca().invert_yaxis()  # Match the image system with origin at top left
        plt.show()

        key = cv2.waitKey(10)
    
    # taper 'q' pour quitter le programme
        if key == cond_quit:
            break

