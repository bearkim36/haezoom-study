import cv2

cap = cv2.VideoCapture("rtsp://192.168.0.103/live/ch00_1");

while True:
    success, img = cap.read()
    cv2.imshow("Face Detection",img)

    if cv2.waitKey(30) & 0XFF == ord('q'):
        break
