import paho.mqtt.client as mqtt
import socks

BROKER_HOST = "mqtt-dashboard.com" 
WEBSOCKET_PORT = 8000              
TOPIC = input("Insira o topico: ")

def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Connected with result code {reason_code}")
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    print(f"Received message on topic '{msg.topic}': {msg.payload.decode()}")

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, transport='websockets')

client.on_connect = on_connect
client.on_message = on_message

try:
    proxy_host = "10.224.200.26"   #<<---------rb proxy
    proxy_port = 8080

    client_id = "clientId-9k3k2mhRx7"

    client.proxy_set(
        proxy_type=socks.HTTP, 
        proxy_addr=proxy_host, 
        proxy_port=proxy_port,
        proxy_username="disrct",
        proxy_password="etsbosch20252"
    )
    # Connect to the broker
    client.connect(BROKER_HOST, WEBSOCKET_PORT, 60)
    
    # Loop forever to process network traffic and receive messages
    client.loop_forever()
    
except KeyboardInterrupt:
    print("Disconnected by user")
    client.disconnect()
except Exception as e:
    print(f"An error occurred: {e}")
