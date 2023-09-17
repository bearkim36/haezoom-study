import cv2
import numpy as np

cap = cv2.VideoCapture("rtsp://192.168.0.20/profile2/media.smp");
kernel = np.ones((5,5), np.uint8)

while True:
    success, img = cap.read()
    imgCanny = cv2.Canny(img, 120, 120)
    imgDilate = cv2.dilate(imgCanny, kernel, iterations=1)
    cv2.imshow("video",imgDilate)

    if cv2.waitKey(30) & 0XFF == ord('q'):
        break
