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



        gray = cv2.cvtColor(output,cv2.COLOR_BGR2GRAY)


        edges = cv2.Canny(gray,100,200,apertureSize = 3)
        cv2.imshow('edges',edges)


        minLineLength = 30
        maxLineGap = 30
        lines = cv2.HoughLinesP(edges,1,np.pi/180,30,minLineLength,maxLineGap)
        if lines is None:
            print 'Pas de lignes'
        else:
            print len(lines)," lignes"
            for x in range(0, len(lines)):
                for x1,y1,x2,y2 in lines[x]:
                    cv2.line(output,(x1,y1),(x2,y2),(0,255,0),2)

            cv2.imshow('hough',output)




        key = cv2.waitKey(10)
    
    # taper 'q' pour quitter le programme
        if key == cond_quit:
            break

