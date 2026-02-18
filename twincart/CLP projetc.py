import pyads as ps
import paho.mqtt.client as mqtt
import socks

ams = "10.234.195.79.1.1"
port = 851
ip = "10.234.197.79"

broker = "mqtt-dashboard.com"
mqttport = 8000

topicEsteira = "dta/esteira"
topicPosition = "dta/robo"
topicTemp = "dta/temp"
topicInfra = "dta/infra"

# proxy_host = "127.0.0.1"   #<<---------rb proxy
proxy_host = "10.224.200.26"   #<<---------rb proxy
proxy_port = 8080

client_id = "clientId-9k3k2mhRx7"

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id)

client.proxy_set(
    proxy_type=socks.HTTP, 
    proxy_addr=proxy_host, 
    proxy_port=proxy_port,
    proxy_username="disrct",
    proxy_password="etsbosch20252"
)

client_connected = False
while(not client_connected):
    try:
        client.connect(broker, mqttport)
        print("conetado")
        client_connected = True
        
        
        position = client.subscribe("/dta/aaws")
        print(position)
        
        
    except Exception as e:
        print(f"sem conexão: {e}")
        i = 0
        while (i < 9999999):
            i+=1

plc = ps.Connection(ams, port)
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