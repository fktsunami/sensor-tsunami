void CheckConnection()
{
	ConnectionCyclesCount++;
	totalCyclesCount++;

	//Restart Ethernet Shield after x connection cycles
	if (ConnectionCyclesCount > maxConnectionCycles)
	{
		DEBUG_PRINT("<CheckConnection> : Restart Ethernet Shield..");
		client.stop();
		Ethernet.begin(mac);
		ConnectionCyclesCount = 0;
	}
	else
	{
		// Daily Softreset
		if (totalCyclesCount > maxCiclosDiarios) {
			DEBUG_PRINT("<CheckConnection> : Reset Device..");
			totalCyclesCount = 0;
			delay(1000);
		//	soft_restart();
		}
		else
		{
			//Check MQTT Connection
			if (!mqttclient.connected()) {
				if (!reconnect()) {
					DEBUG_PRINT("<CheckConnection> : Disconnected.. connection attempt #: " + (String)connectionAttempt);
					connectionAttempt++;
					if (connectionAttempt > maxconnectionAttempt)
					{

						connectionAttempt = 0;
						DEBUG_PRINT("<CheckConnection> : Restart Ethernet!!");
						client.stop();
						Ethernet.begin(mac);
						delay(1000);
					}
				}
				else
				{
					connectionAttempt = 0;
					DEBUG_PRINT("<CheckConnection> : Reconnected!!");
				}
			}
			else
			{
				DEBUG_PRINT("<CheckConnection> : Connected!!");
			}
		}
	}
}
