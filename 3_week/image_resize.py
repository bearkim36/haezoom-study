import cv2

img = cv2.imread("iu.jpg")
dst = cv2.resize(img, dsize=(640, 640), interpolation=cv2.INTER_AREA)
cv2.imshow("iu", dst)

cv2.waitKey(0)

