import paho.mqtt.client as mqtt
from datetime import datetime
import json

tempTemp = [0] * 20
tempTime = [0] * 20
tempHum  = [0] * 20

def on_connect(client, userdata, flags, rc):
    print("Connected with result code", rc)
    client.subscribe("zigbee2mqtt/0x00158d0005373e87")

def on_message(client, userdata, msg):
    print(msg.topic, msg.payload)
    d = json.loads(msg.payload)
    print(d['temperature'])
    tempTemp.append(d['temperature'])
    tempTemp.pop(0)
    tempHum.append(d['humidity'])
    tempHum.pop(0)
    tempTime.append(datetime.now())
    tempTime.pop(0)
    print(tempTemp)
    print(tempHum)


def get_mqtt_message(mqtt_ip="127.0.0.1", mqtt_port=1883):
    client = mqtt.Client(client_id="", clean_session=True, userdata=None, protocol=mqtt.MQTTv311, transport="tcp")
    client.on_connect = on_connect
    client.on_message = on_message

    # client.tls_set()  # <--- even without arguments

    client.username_pw_set(username="usermosquitto", password="Rfhectkm_1")
    print("Connecting...")
    client.connect(mqtt_ip, mqtt_port, 10)
    client.loop_forever()
