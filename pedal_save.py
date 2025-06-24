"""
import serial               
import time
import numpy as np
from datetime import datetime


ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)  

log_data = [] 

last_log_time = 0 

txt_file_name = datetime.now().strftime("%Y-%m-%d %H-%M-%S")


drive_start_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

log_data.append(f"Driving Start : {drive_start_time}")

try:
    
    with open(f"{txt_file_name} : pedal_log.txt", "a") as f:
        f.write(f"Driving Start : {drive_start_time}")
        
    while True:
        line = ser.readline().decode('utf-8').strip()
        if line in ("0", "1"):               
            now = time.time()
            
            if now - last_log_time >= 0.5:
                last_log_time = now
                timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                state = "BRAKE" if line == "0" else "ACCEL"
                

                log_data.append((timestamp, state))

                print(f"{timestamp}: {state}")
                print("페달 기록 완료")

                with open(f"{txt_file_name} - pedal_log.txt", "a") as f:
                    f.write(f"{timestamp},{state}\n")

                # 저장
                np.save(f"{txt_file_name} - pedal_log.npy", np.array(log_data, dtype=object))

            time.sleep(0.1)

except KeyboardInterrupt:
    
    drive_end_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    with open(f"{drive_start_time} : pedal_log.txt", "a") as f:
        f.write(f"Driving End : {drive_end_time}")
    
    np.save(f"{drive_start_time} : pedal_log.npy", np.array(log_data, dtype=object))
    
    print("기록 종료")
    ser.close()"""



import serial
import time
import numpy as np
from datetime import datetime
import os

drive_start = datetime.now().strftime("%Y%m%d_%H%M%S")
log_filename = f"{drive_start}_pedal_log.txt"
npy_filename = f"{drive_start}_pedal_log.npy"

txt_file_path = 'C:/Users/baduk/drive_log_txt'
npy_file_path = 'C:/Users/baduk/drive_log_npy'

txt_total_path = os.path.join(txt_file_path, log_filename)
npy_total_path = os.path.join(npy_file_path, npy_filename)


ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)  

drive_log = []

last_log_time = time.time()

log_file = open(txt_total_path, "a", encoding = "utf-8")

start_log = f"Driving start : {drive_start}"

log_file.write(start_log + "\n")

log_file.flush()
drive_log.append(("START", drive_start))

try:
    while True:
        line = ser.read().decode('utf-8').strip()
        
        if line in ("0", "1"):
            now = time.time()
            
            if now - last_log_time >= 0.5:
                last_log_time = now
                
                last_log_time = now
                timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                state = "BRAKE" if line == "0" else "ACCEL"
                
                drive_log.append((timestamp, state))
                log_file.write(f"{timestamp}, {state}\n")
                log_file.flush()
                
                print(f"{timestamp} : {state} - 기록 완료")
        time.sleep(0.01)
        
except KeyboardInterrupt:
    
    drive_end = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    drive_log.append(("END", drive_end))
    
    log_file.write(f"END, {drive_end}\n")
    
    log_file.close()
    
    np.save(npy_total_path, np.array(drive_log, dtype=object))
    
    print("기록 종료")
    ser.close()
                
                