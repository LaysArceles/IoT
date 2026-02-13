import pyads as ps
import paho.mqtt.client as paho
#from paho import  mqtt
import paho.mqtt.client as mqtt

# TwinkCat e CLP Config
amse = "10.234.195.54.1.1"
porta = 851
ip = "10.234.197.74"

# Configurações do broker
broker = "localhost"      # ou IP do broker
port = 1883

# Tópicos
topicEsp = "dta/esp"
topicEsteira = "dta/esteira"
topicLDR = "dta/LDR"


# Criar cliente
client = mqtt.Client()

# Conectar ao broker
client.connect(broker, port, 60)

# CODIGO PRINCIPAL
plc = ps.Connection(amse, porta)
plc.open()

ledStatus = 0
ledEsteira = 0
ledLDR = 1

esteira = 0
temp = 0

try:
    
    
    
    
    light = plc.read_by_name("GVL.luz", ps.PLCTYPE_INT)
    
    if(light):
        print("A luz está ligada")
        esteira = 1
        client.publish(topicEsteira, "1")
    
    plc.write_by_name("GVL.esteira", esteira, ps.PLCTYPE_INT)
    
    
        
except Exception as e:
    print(f"Ocorreu um erro genérico: {e}")
    
    
# Encerrar conexão MQTT
client.disconnect()

# while True:

#     status = plc.read_by_name("GVL.status", ps.PLCTYPE_INT)
    
#     plc.write_by_name("GVL.posicao", proxima_pos, ps.PLCTYPE_INT)
    

#     garra_val = 1 if proxima_pos in [1, 3] else 0
#     plc.write_by_name("GVL.garra", garra_val, ps.PLCTYPE_INT)
    
#     print(f"Enviado: Posição {proxima_pos} (Passo {n})")
    
#     n += 1
#     time.sleep(2.0) 

#     if n == 8 :
#         break

