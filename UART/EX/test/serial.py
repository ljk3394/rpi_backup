import test.serial as serial
import time

# UART 포트 열기
ser = serial.Serial('/dev/ttyAMA2', 115200)

try:
    while True:
        if ser.in_waiting > 0:
            # 데이터가 도착하면 읽고 출력한다
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
        time.sleep(1)
except KeyboardInterrupt:
    print('프로그램을 종료합니다.')

finally:
    ser.close()