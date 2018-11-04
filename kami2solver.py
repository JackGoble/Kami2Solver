#import OpenCV numpy scipy
import cv2
import numpy as np
import scipy.misc as smp

#1920x1080 picture
#108 px wide per cell
#62 px height
#colors are 430 -> 1080
#sample colors x: 500, 630, 760, 890, 1020

#method to look for similar colors. Due to color variance has a wide range
#compares blue, green, red
def compare(color1, color2):
    boolB = abs((color1[0] - color2[0])) < 45
    boolG = abs((color1[1] - color2[1])) < 45
    boolR = abs((color1[2] - color2[2])) < 45
    return boolB and boolG and boolR

#a few sample pictures
filein = 'kami.png' #3 color triangles
#filein = 'kami2.png' #4 color lines
#filein = 'kami3.png' #2 color hexagons

#arrays for cells and base colors
cells = np.zeros((29,10), dtype=int)
colors = np.zeros((5,3), dtype=int)

#read in file as array of pixels
img = cv2.imread(filein)

#test for colors, sample a pixel in each color key space
colors[0] = img[1850, 500]
colors[1] = img[1850, 630]
colors[2] = img[1850, 760]
colors[3] = img[1850, 890]
colors[4] = img[1850, 1020]
print(colors)

#compare the color keys against each other, "removing" duplicates to determine number of colors
#print(compare(colors[0], colors[1]))
#print(compare(colors[1], colors[2]))
#print(compare(colors[2], colors[3]))
#print(compare(colors[3], colors[4]))

#sum the number of true comparisons, for each true, one fewer color. Zero true is five colors
numColors = 5 - sum([compare(colors[0], colors[1]), compare(colors[1], colors[2]), compare(colors[2], colors[3]), compare(colors[3], colors[4])])
print("Number of colors: " + str(numColors))

#match each pixel to one of the color keys, fill Cells matrix with color nums
#different cases for different number of colors
for h in xrange(0, 29):
    for w in xrange(0, 10):
        if(numColors == 2):  
            if(compare(img[h * 62][w * 108 + 52], colors[0])):
                cells[h][w] = 1
            elif(compare(img[h * 62][w * 108 + 52], colors[4])):
                cells[h][w] = 2
        elif(numColors == 3):  
            if(compare(img[h * 62][w * 108 + 52], colors[0])):
                cells[h][w] = 1
            elif(compare(img[h * 62][w * 108 + 52], colors[2])):
                cells[h][w] = 2
            elif(compare(img[h * 62][w * 108 + 52], colors[4])):
                cells[h][w] = 3
        elif(numColors == 4):  
            if(compare(img[h * 62][w * 108 + 52], colors[0])):
                cells[h][w] = 1
            elif(compare(img[h * 62][w * 108 + 52], colors[1])):
                cells[h][w] = 2
            elif(compare(img[h * 62][w * 108 + 52], colors[2])):
                cells[h][w] = 3
            elif(compare(img[h * 62][w * 108 + 52], colors[4])):
                cells[h][w] = 4
        elif(numColors == 5):  
            if(compare(img[h * 62][w * 108 + 52], colors[0])):
                cells[h][w] = 1
            elif(compare(img[h * 62][w * 108 + 52], colors[1])):
                cells[h][w] = 2
            elif(compare(img[h * 62][w * 108 + 52], colors[2])):
                cells[h][w] = 3
            elif(compare(img[h * 62][w * 108 + 52], colors[3])):
                cells[h][w] = 4
            elif(compare(img[h * 62][w * 108 + 52], colors[4])):
                cells[h][w] = 5

print(cells)

#write to file
outfile = open("kami.txt","w+")
for h in xrange(0, 29):
    for w in xrange(0, 10):
        outfile.write(str(cells[h][w]) + " ")
    outfile.write("\n")

#image printing
#pixels read in as BGR, so swap B and R for RGB usability
for x in xrange(5):
    temp = colors[x][0]
    colors[x][0] = colors[x][2]
    colors[x][2] = temp

#stretching Cells data for clearer picture
data = np.zeros((580,300,3), dtype=int)
for h in xrange(0,580):
    for w in xrange(0,300):
        data[h][w] = colors[cells[h/20][w/30]]
img = smp.toimage(data)
img.show()
