import cv2

cap = cv2.VideoCapture("rtsp://192.168.0.20/profile2/media.smp");

while True:
    success, img = cap.read()
    imgGray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    cv2.imshow("video",imgGray)

    if cv2.waitKey(30) & 0XFF == ord('q'):
        break
