import cv2

cap = cv2.VideoCapture("rtsp://192.168.0.20/profile2/media.smp");

while True:
    success, img = cap.read()
    cv2.imshow("CCTV Video",img)

    if cv2.waitKey(5) & 0XFF == ord('q'):
        break
