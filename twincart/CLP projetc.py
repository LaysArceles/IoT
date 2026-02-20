import pyads as ps
import paho.mqtt.client as mqtt
import socket, socks

# Ctrl Shift P
AMS = "10.234.195.79.1.1"
PORT_CLP = 851
IP = "10.234.197.79"

TOPIC_ESTEIRA = "dta/esteira"
TOPIC_POSITION = "dta/robo"
TOPIC_TEMP = "dta/temp"
TOPIC_INFRA = "dta/infra"

#tem que ligar o rb local proxy manager 
PROXY_HOST = "127.0.0.1"    #<<<<<-------------
PROXY_PORT = 3128           #<<<<<-------------

BROKER = "broker.hivemq.com"
PORT = 8000
WS_PATH = "/mqtt"

socks.setdefaultproxy(socks.HTTP, PROXY_HOST, PROXY_PORT)
socket.socket = socks.socksocket

client = mqtt.Client(
    callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
    client_id="py_sub",
    transport="websockets",
)

# variáveis globais para armazenar dados recebidos
temperatura = 0
infra = 0
position = 0
esteira = 0

def on_connect(client, userdata, flags, reason_code, properties=None):
    print("Conectado")
    if reason_code == 0:
        client.subscribe(TOPIC_TEMP)
        client.subscribe(TOPIC_INFRA)
        client.subscribe(TOPIC_POSITION)

def on_message(client, userdata, msg):
    global temperatura, infra, position

    payload = msg.payload.decode(errors='ignore')
    print(f"{msg.topic} -> {payload}")
    
    valor = payload

    if msg.topic == TOPIC_TEMP:
        try:
            valor = float(payload)
        except ValueError:
            print("Payload inválido, não é número.")
            return
        temperatura = valor
        verification(temperatura,infra)

    elif msg.topic == TOPIC_INFRA:
        infra = valor
        verification(temperatura,infra)

    elif msg.topic == TOPIC_POSITION:
        position = valor
        
def ligarRobo():
    print("Ligando Robo")
        
def verification(temperatura,infra):
    
    # Se a esteira estiver ligada e o objeto no fim, liga robo.
    if (esteira == 0 and infra == "OBJETO_FIM"):
        ligarRobo()
    
    # Liga/Desliga a Esteira 
    if (temperatura < 35 and infra == "OBJETO_INICIO"):
        if (esteira == 0):
            to_publish(TOPIC_ESTEIRA, "LIGADA")
            esteira = 1
            print("Ligando Esteira!")
    else:
        if (esteira == 1):
            to_publish(TOPIC_ESTEIRA, "DESLIGADA")
            esteira = 0
            print("Desligando Esteira!")

def to_publish(topico, payload): #não testei muito bem essa função ainda
    client.publish(topico, payload)
    print("enviei info: {}".format(payload))
    # print(f"enviei info: {payload}")
    
client.ws_set_options(path=WS_PATH)
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT, 20)
client.loop_start()

to_publish(TOPIC_POSITION, "testando") #verificar se cria uma lógica bloqueante ainda funcionará

plc = ps.Connection(AMS, PORT_CLP)
plc.open()

allConnected = 0

while True:
    
    print("")
    print("----------------")
    print(f"Infra: {infra}")
    print(f"Temp: {temperatura}")
    print("----------------")
    
    try:
        plc.write_by_name("GVL.position", position, ps.PLCTYPE_INT)
        print(f"Position: {position}")
        
        plc.write_by_name("GVL.infra", infra, ps.PLCTYPE_INT)
        print(f"Infra: {infra}")
        
        plc.write_by_name("GVL.temp", temperatura, ps.PLCTYPE_INT)
        print(f"Temp: {temperatura}")
        
        if (temperatura < 35):
            client.publish(TOPIC_ESTEIRA, "1")
            plc.write_by_name("GVL.esteira", 1, ps.PLCTYPE_INT)
        else:
            client.publish(TOPIC_ESTEIRA, "0")
            plc.write_by_name("GVL.esteira", 0, ps.PLCTYPE_INT)

    except Exception as e:
        print(f"Ocorreu um erro genérico: {e}")