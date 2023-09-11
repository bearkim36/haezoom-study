import cv2

cap = cv2.VideoCapture("video36.avi");

while True:
    success, img = cap.read()
    imgGray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    cv2.imshow("video",imgGray)

    if cv2.waitKey(30) & 0XFF == ord('q'):
        break
