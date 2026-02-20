import pyads as ps
import paho.mqtt.client as mqtt
import socket, socks

ams = "10.234.195.79.1.1"
portClp = 851
ip = "10.234.197.79"

topicEsteira = "dta/esteira"
topicPosition = "dta/robo"
topicTemp = "dta/temp"
topicInfra = "dta/infra"

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

def to_publish(topico, payload): #não testei muito bem essa função ainda
    client.publish(TOPICO, payload)
    print("enviei info: {}".format(payload))
    # print(f"enviei info: {payload}")
    
client.ws_set_options(path=WS_PATH)
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT, 20)
client.loop_forever()
to_publish(TOPICO, "testando") #verificar se cria uma lógica bloqueante ainda funcionará


plc = ps.Connection(ams, portClp)
plc.open()

allConnected = 0

while True:
    print("----------------")
    i = 0
    while (i < 9999999):
        i+=1
    try:
        position = client.subscribe(topicPosition)
        plc.write_by_name("GVL.position", position[0], ps.PLCTYPE_INT)
        print(f"Position: {position[0]}")
        print(f"Position: {position}")
        
        infra = client.subscribe(topicInfra)
        plc.write_by_name("GVL.infra", infra[0], ps.PLCTYPE_INT)
        print(f"Infra: {infra[0]}", end="")
        print(f"Infra: {infra}")
        
        temp = client.subscribe(topicTemp)[0]
        plc.write_by_name("GVL.temp", temp, ps.PLCTYPE_INT)
        print(f"Temp: {temp}")
        
        if (temp < 35):
            client.publish(topicEsteira, "1")
            plc.write_by_name("GVL.esteira", 1, ps.PLCTYPE_INT)
        else:
            client.publish(topicEsteira, "0")
            plc.write_by_name("GVL.esteira", 0, ps.PLCTYPE_INT)
            
    except Exception as e:
        print(f"Ocorreu um erro genérico: {e}")
    finally:
        client.disconnect()