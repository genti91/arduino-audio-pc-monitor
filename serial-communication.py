import requests
import serial
import time


ser = serial.Serial("COM2", 9600)
url = "http://192.168.0.98:8085/data.json"
float_temp = 0
check_load = 0
float_gtemp = 0
check_gload = 0
red = 0
green = 0
blue = 0
ored = 0
ogreen = 0
oblue = 0

last_request_time = time.time()
min_interval = 0.5

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode(encoding='ISO-8859-1')
        if line.startswith("*"):
            #print("Invalid color string format")
            parts = line[1:].split("|")
            if (red != parts[0] and int(parts[0]) != int(red)+1 and int(parts[0]) != int(red)-1) or (green != parts[1] and int(parts[1]) != int(green)+1 and int(parts[1]) != int(green)-1) or (blue != parts[2] and int(parts[2]) != int(blue)+1 and int(parts[2]) != int(blue)-1):
                red = parts[0]
                green = parts[1]
                blue = parts[2]
                if red == "1":
                    red = "0"
                if green == "1":
                    green = "0"
                if blue == "1":
                    blue = "0"
                current_time = time.time()
                if current_time - last_request_time >= min_interval:
                    print(red + "|" + green + "|" + blue)
                    try:
                        requests.get("http://192.168.0.88:8080/?blue=" + green)
                        requests.get("http://192.168.0.88:8080/?green=" + red)
                        requests.get("http://192.168.0.88:8080/?red=" + blue)
                        ogreen = green
                        ored = red
                        oblue = blue
                    except:
                        print("Error senndig data to esp8266")
                    last_request_time = current_time
                    

    if time.time() - last_request_time >= min_interval:
        if red != ored or green != ogreen or blue != oblue:
            try:
                requests.get("http://192.168.0.88:8080/?blue=" + green)
                requests.get("http://192.168.0.88:8080/?green=" + red)
                requests.get("http://192.168.0.88:8080/?red=" + blue)
                ogreen = green
                ored = red
                oblue = blue
                print("Updated old values")
                print(red + "|" + green + "|" + blue)

            except:
                print("Error senndig data to esp8266")


    # response = requests.get(url)
    # if response.status_code == 200:
    #     data = response.json()
    #     temp = data['Children'][0]['Children'][0]['Children'][1]['Children'][0]['Value']
    #     temp = int(float(temp.replace(" °C", "")))
    #     load = data['Children'][0]['Children'][0]['Children'][2]['Children'][0]['Value']
    #     load = str(int(float(load.strip(" %")))) + "%"
    #     gtemp = data['Children'][0]['Children'][2]['Children'][2]['Children'][0]['Value']
    #     gtemp = int(float(gtemp.replace(" °C", "")))
    #     gload = data['Children'][0]['Children'][2]['Children'][3]['Children'][0]['Value']
    #     gload = str(int(float(gload.strip(" %")))) + "%"
    #     if temp != float_temp or load != check_load or gtemp != float_gtemp or gload != check_gload:
    #         chack_gload = gload
    #         float_gtemp = gtemp
    #         float_temp = temp
    #         check_load = load
    #         values = str(temp) + " " + load + " " + str(gtemp) + " " + gload + "|"
    #         ser.write(str(values).encode())
    # else:
    #     print("Failed to retrieve data. Response status code:", response.status_code)

    #time.sleep(0.5)

ser.close()
