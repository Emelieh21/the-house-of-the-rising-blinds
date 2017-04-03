var mqtt = require('mqtt')
var request = require('request');

var flag = false;

var client = mqtt.connect({
  servers:[{'host':'mqtt.relayr.io'}],
  username: "<YOUR_DEVICE_USERNAME_HERE>",
  password: "<YOUR_DEVICE_PASSWORD_HERE>",
  protocol : 'mqtts'
});

client.on('connect', function() {

  //subscribe to commands sent from the dashboard or other clients
  client.subscribe("<YOUR_TOPIC_HERE>/data");

  client.on('message', function(topic, message) {
  	
  	console.log(message.toString())
	var obj = JSON.parse(message);

	var sunStatus = obj[0].value;

	//DEBUG: Prints the status of the door
	console.log(sunStatus);
	
	if (flag == false){
		if (sunStatus < 50){
			flag = true;
			console.log("");
			request('http://<IP_OF_YOUR_SONOS_SERVER_HERE>:5005/<SONOS_ROOM_NAME>/pause', function (error, response, body) {
				if (!error && response.statusCode == 200) {
					console.log(body)
				}
			})
		}
	}
	if (flag == true){
		if (sunStatus > 50){
			flag = false;
			console.log("");
			request('http://<IP_OF_YOUR_SONOS_SERVER_HERE>:5005/<SONOS_ROOM_NAME>/play', function (error, response, body) {
				if (!error && response.statusCode == 200) {
					console.log(body)
				}
			})
		}
	}
	console.log(sunStatus)
	
  	
  });
  
});

