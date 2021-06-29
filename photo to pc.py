import serial

BAUD = 115200
PORT = "COM9"
TIMEOUT = 1

uart = serial.Serial(PORT, baudrate=BAUD, timeout=TIMEOUT)

photo = b''

while 1:
    try:
        reply = uart.read(106)
        print(reply[5:-5])
        photo += reply[5:-5]
    except KeyboardInterrupt:
        break

f = open("photo.jpg", "wb")
f.write(photo)
f.close()
