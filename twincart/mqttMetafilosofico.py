import socket, socks
import paho.mqtt.client as mqtt

#tem que ligar o rb local proxy manager 
PROXY_HOST = "127.0.0.1"    #<<<<<-------------
PROXY_PORT = 3128           #<<<<<-------------

BROKER = "broker.hivemq.com"
PORT = 8000
WS_PATH = "/mqtt"

TOPICO = "dta/robo"

socks.setdefaultproxy(socks.HTTP, PROXY_HOST, PROXY_PORT)
socket.socket = socks.socksocket

client = mqtt.Client(
    callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
    client_id="py_sub",
    transport="websockets",
)

def on_connect(client, userdata, flags, reason_code, properties=None):
    print("connect:", reason_code)
    if reason_code == 0:
        client.subscribe(TOPICO)
        print("sub:", TOPICO)

def on_message(client, userdata, msg):
    print(f"{msg.topic} -> {msg.payload.decode(errors='ignore')}")

# def to_publish(topico, payload): #não testei muito bem essa função ainda
#     client.publish(TOPICO, payload)
#     print("enviei info: {}".format(payload))
    
# to_publish(TOPICO, "testando") #verificar se criar uma lógica bloqueante ainda funcionará
client.ws_set_options(path=WS_PATH)
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT, 20)
client.loop_forever()
