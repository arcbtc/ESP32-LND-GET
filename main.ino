
#include <WiFiClientSecure.h>

const char* ssid     = "yourssid";     // your network SSID (name of wifi network)
const char* password = "yourpass"; // your network password

const char*  server = "yournode.com";  // Node URL

String readmac = "your read macaroon as a string";

const char* test_root_ca = \ // Needs to be in this specific format, you can export SSL from firefox
    "-----BEGIN CERTIFICATE-----\n" \
    "MIICBTCCAaqgAwIBAgIQBSMZ9g3niBo1jyzK1DvECDAKBggqhkjOPQQDAjAyMR8w\n" \
    "HQYDVQQKExZsbmQgYXV0b2dlbmVyYXRlZCBjZXJ0MQ8wDQYDVQQDEwZSb29tNzcw\n" \
    "HhcNMTkwNTE2MjAwODUwWhcNMjAwNzEwMjAwODUwWjAyMR8wHQYDVQQKExZsbmQg\n" \
    "YXV0b2dlbmVyYXRlZCBjZXJ0MQ8wDQYDVQQDEwZSb29tNzcwWTATBgcqhkjOPQIB\n" \
    "BggqhkjOPQMBBwNCAAQJ3795aca5YP6AaUtcvlSqopqR7ePzLshQoTUeV6FVKbFC\n" \
    "CC+fVGRQsXJx+GVUknnNEcJTt/fQ9CmM6stqGPjAo4GhMIGeMA4GA1UdDwEB/wQE\n" \
    "DVQQDEwZSb29tNzcwWTATBgcqhkjOPQIA1UdEQR0MHKCBlJvb203N4IJbG9jYWxo\n" \
    "b3N0ghVyb29tNzcucmFzcGlibGl0ei5jb22CBHVuaXiCCnVuaXhwYWNrZXSHBH8A\n" \
    "AAGHEAAAAAAAAAAAAAAAAAAAAAGHBMCoskqHEP6AAAAAAAAA+OWZZHfUV0qHBAAA\n" \
    "AAAwCgYIKoZIzj0EAwIDSQAwRgIhALKz3oScii3i+5ltMVQc9u2O38rgfnGCj5Lh\n" \
    "u9iwcAiZAiEA0BjRcisPUlG+SE/s+x6/A2NuT0gtIZ3PKd/GuM5T0jM=\n" \
    "-----END CERTIFICATE-----\n";

WiFiClientSecure client;

void setup() {

  Serial.begin(115200);
  delay(100);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Connected to ");
  Serial.println(ssid);

  client.setCACert(test_root_ca);
  
  Serial.println("\nStarting connection to server...");
  if (!client.connect(server, 8077))
    Serial.println("Connection failed!");
  else {
    Serial.println("Connected to server!");

    client.println("GET https://yournode.com:8077/v1/getinfo HTTP/1.0");
    client.println("Host: yournode.com:8077");
    client.println("Grpc-Metadata-macaroon:" + readmac );
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }

    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  }
}

void loop() {


}
