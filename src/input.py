import serial
import sys
import keyboard

if len(sys.argv) < 2: 
    print("Please provide a port (COM*) and baud")
    sys.exit(1)

port = serial.Serial(sys.argv[1], int(sys.argv[2]))

while True:
    if keyboard.is_pressed('q'): break
    output = ""
    if not keyboard.is_pressed('c'):
        x, y = 5, 5
        if keyboard.is_pressed('w'): y += 1
        if keyboard.is_pressed('a'): x -= 1
        if keyboard.is_pressed('s'): y -= 1
        if keyboard.is_pressed('d'): x += 1
        output = "</wm" + str(x) + str(y) + ">"
    else:
        x = 1
        if keyboard.is_pressed('w'): x += 1
        if keyboard.is_pressed('d'): x += 1
        output = "</cm" + str(x) + ">"

    port.write(output.encode())