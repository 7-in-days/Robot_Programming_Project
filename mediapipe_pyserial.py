import cv2
import mediapipe as mp
from scipy.spatial import distance
import numpy as np
import serial
import time

ser = serial.Serial('COM7', 9600, timeout=1) # 포트 번호는 조정해야함.
time.sleep(2)  # Arduino 리셋 대기


mp_face = mp.solutions.face_mesh
mesh = mp_face.FaceMesh(
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5
)

LEFT_IDS  = [33,160,158,133,153,144]
RIGHT_IDS = [362,385,387,263,373,380]

EYE_AR_THRESH          = 0.3
EYE_CLOSED_TIME_THRESH = 3.0   # 눈 감김 임계 시간

closed_start_time = None
flag_sent = False

def eye_aspect_ratio(lm, ids, w, h):
    pts = [(int(lm.landmark[i].x*w), int(lm.landmark[i].y*h)) for i in ids]
    A = distance.euclidean(pts[1], pts[5])
    B = distance.euclidean(pts[2], pts[4])
    C = distance.euclidean(pts[0], pts[3])
    return (A + B) / (2.0 * C), pts

cap = cv2.VideoCapture(0)
while True:
    ret, img = cap.read()
    if not ret: break

    h, w, _ = img.shape
    results = mesh.process(cv2.cvtColor(img, cv2.COLOR_BGR2RGB))

    if results.multi_face_landmarks:
        lm = results.multi_face_landmarks[0]
        l_ear, l_pts = eye_aspect_ratio(lm, LEFT_IDS,  w, h)
        r_ear, r_pts = eye_aspect_ratio(lm, RIGHT_IDS, w, h)
        ear = (l_ear + r_ear) / 2.0
        
        cv2.polylines(img, [np.array(l_pts, dtype=np.int32)], True, (255,255,0), 1)
        cv2.polylines(img, [np.array(r_pts, dtype=np.int32)], True, (255,255,0), 1)
        for (x,y) in l_pts + r_pts:
            cv2.circle(img, (x,y), 2, (0,255,255), -1)
        
        color = (0,255,0) if ear >= EYE_AR_THRESH else (0,0,255)
        cv2.putText(img, f"EAR:{ear:.2f}", (30,30),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.8, color, 2)

        now = time.time()
        if ear < EYE_AR_THRESH:
            if closed_start_time is None:
                closed_start_time = now
                flag_sent = False
            else:
                elapsed = now - closed_start_time
                if elapsed >= EYE_CLOSED_TIME_THRESH and not flag_sent:
                    ser.write(b'C')
                    print("[PY] Sent C (closed for 3s)")
                    flag_sent = True
        else:
            if closed_start_time is not None and flag_sent:
                ser.write(b'O')
                print("[PY] Sent O (open)")
            closed_start_time = None
            flag_sent = False

    cv2.imshow("Drowsiness Detection", img)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
ser.close()


