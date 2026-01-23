#include <PubSubClient.h>

extern unsigned long tempo;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("MSG recebida no tópico: ");
  Serial.println(topic);
  String msg = "";
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.println("Conteudo: " + msg);
  if (msg == "LIGA") {                                                            
    digitalWrite(LED_PIN, 0); 
    client.publish(topico_status, "LIGADO", true);
  } 
  else if (msg == "DESLIGA") {                                                    
    digitalWrite(LED_PIN, 1);
    client.publish(topico_status, "DESLIGADO", true);
  }

  int posi = msg.indexOf(":");
String MsgS;
int MsgN;

  if (posi != -1){
    String MsgS = msg.substring(0,posi);
    msg.toUpperCase();
    int MsgN = msg.substring(posi + 1).toInt();
    MsgS.trim();
  }

  
 int cor = -1;

if (MsgS == "RED"){
  analogWrite(Green,255);
  analogWrite(Red,0);
  analogWrite(Blue,255);
} 
else if (MsgS == "GREEN") {
  analogWrite(Green,0);
  analogWrite(Red,255);
  analogWrite(Blue,255);

}else if (MsgS == "BLUE") {
  analogWrite(Green,255);
  analogWrite(Red,255);
  analogWrite(Blue,0);
}


  // switch(cor){
  //     case 1:
  //         analogWrite(Green,255);
  //         analogWrite(Red,0);
  //         analogWrite(Blue,255);
  //         break;
  //     case 2:
  //         analogWrite(Green,255);
  //         analogWrite(Red,255);
  //         analogWrite(Blue,0);
  //         break;
  //     case 3:
  //       analogWrite(Green,0);
  //       analogWrite(Red,255);
  //       analogWrite(Blue,255);
  //       break;
  //     default:
  //       break;
  // }

}

boolean attemptMqttConnection() {
  String clientId = String(mqtt_client_id) + "_" + String(WiFi.macAddress());
  Serial.print("Tentando MQTT como: " + clientId + "... ");
  if (client.connect(clientId.c_str(), NULL, NULL, topico_status, 0, true, "OFFLINE")){
    Serial.println("CONECTADO!");
    client.publish(topico_status, "ONLINE", true);
    client.subscribe(topico_comando);
    client.subscribe(topico_texto);
    return true;
  } else {
    Serial.print("Falha. rc=");
    Serial.print(client.state());
    return false;
  }
}

void taConectado(){
  if (!client.connected()) {
    isConnected = false;
    if (tempo > 5000) {
      tempo = 0;
      if (attemptMqttConnection()) tempo = 0;
    }
  } else {
    isConnected = true;
    client.loop();
  }
}