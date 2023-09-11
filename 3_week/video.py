import cv2

cap = cv2.VideoCapture("video36.avi");

while True:
    success, img = cap.read()
    cv2.imshow("Face Detection",img)

    if cv2.waitKey(5) & 0XFF == ord('q'):
        break
