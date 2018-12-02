
boolean reconnect() {
	if (mqttclient.connect("arduinoClient")) {

		char topicConnection[50];

		subscriptionTopic.toCharArray(topicConnection, 25);
		mqttclient.subscribe(topicConnection);
	}

	return mqttclient.connected();
}
char* getIpReadable(IPAddress ipAddress)
{
	//Convert the ip address to a readable string
	unsigned char octet[4] = {
	  0, 0, 0, 0 };
	for (int i = 0; i < 4; i++)
	{
		octet[i] = (ipAddress >> (i * 8)) & 0xFF;
	}

	sprintf(myIPAddress, "%d.%d.%d.%d\0", octet[0], octet[1], octet[2], octet[3]);

	return myIPAddress;
}

void DecodeJson(char json[]) {
	StaticJsonBuffer<200> jsonBuffer;

	JsonObject& root = jsonBuffer.parseObject(json);

	if (!root.success())
	{
		DEBUG_PRINT("<DecodeJson> parseObject() failed");
		return;
	}

	const char* mensaje = root["message"];
	long data = root["valor"];

}
void ProcessPayload(char* chararray) {

	size_t charSize;


	// Extract Json
	StaticJsonBuffer<200> jsonBuffer;

	JsonObject& root = jsonBuffer.parseObject(chararray);

	if (!root.success())
	{
		DEBUG_PRINT("<DecodeJson> parseObject() failed");
		return;
	}

	for (JsonObject::iterator it = root.begin(); it != root.end(); ++it)
	{
		DEBUG_PRINT(it->key);
		DEBUG_PRINT(it->value.asString());
	}


	delay(1000);

	//Publish ACK
	Publicar(publishTopic, "ok", true);

}


//Function to Publish to MQTT Broker
//Parameters
//      topic : ..
//      value  : El valor que se va a publicar en el broker
//      retain:  (UNSUPPORTED) If we want to retain the value

bool Publicar(String topic, String value, bool retain)
{

	bool success = false;
	char cpytopic[50];
	char message[50];
	value.toCharArray(message, value.length() + 1);
	topic.toCharArray(cpytopic, topic.length() + 1);
	success = mqttclient.publish(cpytopic, message);

	return success;
}


