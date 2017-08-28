/*
 
  Original Author: Karl THOMAS https://github.com/karlth
  
*/

#ifndef DUINOEDU_ESP8266_H
#define DUINOEDU_ESP8266_H

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>



class Duinoedu_Esp8266	
{
  public:
    Duinoedu_Esp8266(){;}
	String DUINOEDU_IP;
	String DUINOEDU_SSID;
	String DUINOEDU_PWD;


int Received( Adafruit_MQTT_Subscribe &Feed , Adafruit_MQTT_Subscribe *subs)
{
	 if (subs == &Feed) {
		int Value = atoi((char *)&Feed.lastread);
		 return Value;
    }
}
	
void mqttpublish(Adafruit_MQTT_Publish Feed, String Flux ,int Value ) {
  Serial.print("Sending Feed :");
  Serial.print(Flux);
  Serial.print(" val :");
  Serial.print(Value);
  Serial.print("...");
  if (! Feed.publish(Value)) {
    Serial.println(F("Failed"));
  } 
  else {
    Serial.println(F("OK!"));
  }
}	
	
	
void mqqtconnect_Adafruit(Adafruit_MQTT_Client *_mqtt  ){
  int8_t ret;

  // Stop if already connected.
  if (_mqtt->connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = _mqtt->connect()) != 0) { // connect will return 0 for connected
    Serial.println(_mqtt->connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 10 seconds...");
    _mqtt->disconnect();
    delay(10000);  // wait 10 seconds
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
	Serial.println("MQTT Connected!");
	if(! _mqtt->ping()) {
    _mqtt->disconnect();
  }
	
}	
	
void connect_AP(const char *ssid,const char *password ){
	Serial.begin(9600);
	delay(500);
	WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint;
	WiFi.softAP( ssid, password ); //The command will only work if the password length is 8 characters or more.
	Serial.print ( "Connected to access point : " ); 
	Serial.println ( ssid );  
	Serial.print ( "IP address: " ); 
	DUINOEDU_IP=WiFi.softAPIP().toString();
	DUINOEDU_SSID=ssid;
	DUINOEDU_PWD=password;
	Serial.println ( WiFi.softAPIP() );
}


void connect_AP_SetIP(const char *ssid,const char *password, IPAddress _local_ip, IPAddress _subnet, IPAddress  _gateway){
	Serial.begin(9600);
	delay(500);
	WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint;
	WiFi.softAP( ssid, password ); //The command will only work if the password length is 8 characters or more.
	WiFi.softAPConfig(_local_ip, _gateway, _subnet);
	Serial.print ( "Connected to access point : " ); 
	Serial.println ( ssid );  
	Serial.print ( "IP address: " ); 
	DUINOEDU_IP=WiFi.softAPIP().toString();
	DUINOEDU_SSID=ssid;
	DUINOEDU_PWD=password;
	Serial.println ( WiFi.softAPIP() );
}

void connect_STA(const char *ssid,const char *password ) {
	Serial.begin(9600);
	WiFi.mode(WIFI_STA);
	WiFi.begin ( ssid, password ); 
	while ( WiFi.status() != WL_CONNECTED ) {  
	delay ( 500 ); 
	Serial.print ( "." ); 
	}
	Serial.println ( "" ); 
	Serial.print ( "Connected to station : " ); 
	Serial.println ( ssid );  
	Serial.print ( "IP address: " ); 
	DUINOEDU_IP=WiFi.localIP().toString();
	DUINOEDU_SSID=ssid;
	DUINOEDU_PWD=password;
	Serial.println ( WiFi.localIP() );
	delay ( 500 );
}




void connect_STA_SetIP(const char *ssid,const char *password, IPAddress _local_ip, IPAddress _subnet, IPAddress  _gateway) {
	Serial.begin(9600);
	WiFi.mode(WIFI_STA);
	WiFi.begin ( ssid, password ); 
	WiFi.config(_local_ip, _gateway, _subnet);
	while ( WiFi.status() != WL_CONNECTED ) {  
	delay ( 500 ); 
	Serial.print ( "." ); 
	}
	Serial.println ( "" ); 
	Serial.print ( "Connected to station : " ); 
	Serial.println ( ssid );  
	Serial.print ( "IP address: " ); 
	DUINOEDU_IP=WiFi.localIP().toString();
	DUINOEDU_SSID=ssid;
	DUINOEDU_PWD=password;
	Serial.println ( WiFi.localIP() );
	delay ( 500 );
}

void sendHttpRequest( String _serverHost,String _name, String _data ) {
  String _send;
  HTTPClient http;
  _serverHost="http://"+_serverHost+"/";
  http.begin(_serverHost);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  _send = _name+ "="+  _data;
  Serial.println("- data stream: "+_send);
  http.POST(_send);
  http.writeToStream(&Serial);
  http.end();
}


void updateStringint(ESP8266WebServer *_server, String _etiquette, int &_varDeStockage){
  if ( _server->hasArg(_etiquette )){
    _varDeStockage= (_server->arg(_etiquette).toInt());
    Serial.print(_etiquette);
    Serial.print(":");
    Serial.println(_varDeStockage);
  }
}

void updateStringbool(ESP8266WebServer *_server, String _etiquette, boolean &_varDeStockage){
  if ( _server->hasArg(_etiquette) ){
    if ((_server->arg(_etiquette))==("0")) { _varDeStockage = 0;}
    else { _varDeStockage = 1;}
    Serial.print(_etiquette);   
    Serial.print(":");
    Serial.println(_varDeStockage);
  }
}

void updateStringdouble (ESP8266WebServer *_server, String _etiquette, float _varDeStockage){
	
	if ( _server->hasArg(_etiquette )){
    _varDeStockage=((_server->arg(_etiquette))).toFloat() ;
    Serial.print(_etiquette);
    Serial.print(":");
    Serial.println(_varDeStockage);
	
	}
}

void updateString(ESP8266WebServer *_server, String _etiquette, String &_varDeStockage){
  if ( _server->hasArg(_etiquette )){
    _varDeStockage= (_server->arg(_etiquette));
    Serial.print(_etiquette);
    Serial.print(":");
    Serial.println(_varDeStockage);
  }
}

String gauge(int _Min, int _Max, String _Title){
	return "<p><script src='https://canvas-gauges.com/download/latest/radial/gauge.min.js'></script><h3>"+_Title+"</h3><canvas id='"+_Title+"'></canvas><script>    var gauge"+_Title+" = new RadialGauge({renderTo: '"+_Title+"', width: 200, height: 200, barWidth: 5, colorBarProgress: '#00FF00', minValue: "+String(_Min)+", maxValue: "+String(_Max)+", majorTicks: ['"+String(_Min)+"','"+String(_Max)+"'],animation: true,animationRule: 'linear',animateOnInit: true,animatedValue: true,listeners: { value: function(value) { this.update({ units: parseInt(value, 10) }); }}}).draw();</script></p>";	
}

String iframe(String _Url, int _Width, int _Height){
	return "<p><iframe style='border: 1px #FFFFFF none;' src='+_Url+' width='"+String(_Width)+"px' height='"+String(_Height)+"px' frameborder='1' marginwidth='0px' marginheight='0px' scrolling='yes'></iframe></p>";
}
String javaslider() {
	return "<script type=\"text/javascript\">\r\nfunction sendDac(pin,value){\r\nserver =  \"http://"+DUINOEDU_IP+"/?\"+pin+\"=\"+value;\r\nrequest = new XMLHttpRequest();\r\nrequest.open(\"GET\", server, true);\r\nrequest.send();\r\nValNum = \"value\" + pin;\r\ndocument.getElementById(ValNum).innerHTML=value;\r\n}\r\n</script>\r\n";
}
String slider(int _Min, int _Max, String _Title){
	return "<tr>\r\n<td align=\'center\'>\r\n"+_Title+"\r\n <br>\r\n<input type=\'range\' style=\'width: 90px; height: 30px;\' id=\'"+_Title+"\' min=\'"+String(_Min)+"\' max=\'"+String(_Max)+"\' value=\'0\'step=\'1\' onchange=\'sendDac(\""+_Title+"\",this.value);\'/>\r\n<br>\r\n<span id=\'value"+_Title+"\'>0</span>\r\n</td>\r\n</tr>\r\n<hr>\r\n";
}
	
String javaScript_start(){
	String javaScript="<SCRIPT>\n";
	javaScript+="var xmlHttp=createXmlHttpObject();\n";
	javaScript+="function createXmlHttpObject(){\n";
	javaScript+=" if(window.XMLHttpRequest){\n";
	javaScript+="    xmlHttp=new XMLHttpRequest();\n";
	javaScript+=" }else{\n";
	javaScript+="    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
	javaScript+=" }\n";
	javaScript+=" return xmlHttp;\n";
	javaScript+="}\n";
	javaScript+="function process(){\n";
	javaScript+=" if(xmlHttp.readyState==0 || xmlHttp.readyState==4){\n";
	javaScript+="   xmlHttp.open('PUT','xml',true);\n";
	javaScript+="   xmlHttp.onreadystatechange=handleServerResponse;\n"; // no brackets?????
	javaScript+="   xmlHttp.send(null);\n";
	javaScript+=" }\n";
	javaScript+=" setTimeout('process()',500);\n";
	javaScript+="}\n";
	javaScript+="function handleServerResponse(){\n";
	javaScript+=" if(xmlHttp.readyState==4 && xmlHttp.status==200){\n";
	javaScript+="   xmlResponse=xmlHttp.responseXML;\n";	
	return javaScript;
}

String javaScript_end(){
	String javaScript=" }\n";
	javaScript+="}\n";
	javaScript+="</SCRIPT>\n";
	return javaScript;
}
	
 };

#endif