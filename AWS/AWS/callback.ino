void callback(char* topic, byte* payload, unsigned int length) {

	/*
	Internally, the client uses the same buffer for both inbound and outbound messages. After the callback function returns,
	or if a call to either publish or subscribe is made from within the callback function, the topic and payload values passed
	to the function will be overwritten. The application should create its own copy of the values if they are required beyond this.
	*/

	// Allocate the correct amount of memory for the payload copy
	byte* p = (byte*)malloc(length);
	// Copy the payload to the new buffer
	memcpy(p, payload, length);

	char* chararray;
	chararray = (char*)p;

	ProcessPayload(chararray);

	free(p);
}
