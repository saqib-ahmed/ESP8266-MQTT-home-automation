//Using the HiveMQ public Broker, with a random client Id
 var client = new Messaging.Client("broker.mqtt-dashboard.com", 8000, "myclientid_" + parseInt(Math.random() * 100, 10));

 //Gets called on page load
 $(document).ready(function(){
      client.connect(options);
});

 //Gets  called if the websocket/mqtt connection gets disconnected for any reason
 client.onConnectionLost = function (responseObject) {
     //Depending on your scenario you could implement a reconnect logic here
     console.log("connection lost: " + responseObject.errorMessage);
	 document.getElementById("status").style.backgroundColor = "#bc0000";
	 document.getElementById("p1").innerHTML = "Offline";
	 client.connect(options);
 };
 
 //Connect Options
 var options = {
     timeout: 3,
     //Gets Called if the connection has sucessfully been established
     onSuccess: function () {
         console.log("Connected");
		 client.subscribe('EEL/client_001/#', {qos: 0}); 
		 console.log('Subscribed');
		 document.getElementById("status").style.backgroundColor = "#009700";
		 document.getElementById("p1").innerHTML = "Online";
		 
     },
     //Gets Called if the connection could not be established
     onFailure: function (message) {
         console.log("Connection failed: " + message.errorMessage);
		 document.getElementById("status").style.backgroundColor = "#bc0000";
		 document.getElementById("p1").innerHTML = "Offline";
		 client.connect(options);
     }
 };

 //Creates a new Messaging.Message Object and sends it to the HiveMQ MQTT Broker
 var publish = function (payload, topic, qos) {
     //Send your message (also possible to serialize it as JSON or protobuf or just use a string, no limitations)
     var message = new Messaging.Message(payload);
     message.destinationName = topic;
     message.qos = qos;
     client.send(message);
 }
 
 //Functions to change state of switch
function check(id) {
    document.getElementById(id).checked = true;
}

function uncheck(id) {
    document.getElementById(id).checked = false;
}

 //Function for state of switch
 function switchValue(id,topic)
{
$(function() {
  $(id).on('change', function(e) {
    e.stopPropagation();
    this.value = this.checked ? 1 : 0;
	console.log(this.value);
	publish(this.value,topic,0)
  });
})
}

 //Gets called whenever you receive a message for your subscriptions
 client.onMessageArrived = function (message) {
	 console.log("Incoming Message");
     //Do something with the push message you received
     $('#messages').append('<span>Topic: ' + message.destinationName + '  | ' + message.payloadString + '</span><br/>');
	 if(message.destinationName=="EEL/client_001/LED1")
		{
			console.log("LED1");
			if(message.payloadString==1)
			{
				check("LED1");
				document.getElementById('LED1_f').src='light_on.svg';
				console.log("LED1 On");
			}
			else if(message.payloadString==0)
			{
				uncheck("LED1");
				document.getElementById('LED1_f').src='light_off.svg';
				console.log("LED1 Off");
			}
		}
		
		
	 else if(message.destinationName=="EEL/client_001/LED2")
		{
			console.log("LED2");
			if(message.payloadString==1)
			{
				check("LED2");
				document.getElementById('LED2_f').src='fan_on.svg';
				console.log("LED2 On");
			}
			else if(message.payloadString==0)
			{
				uncheck("LED2");
				document.getElementById('LED2_f').src='fan_off.svg';
				console.log("LED2 Off");
			}
		}
		
	 else if(message.destinationName=="EEL/client_001/LED3")
		{
			console.log("LED3");
			if(message.payloadString==1)
			{
				check("LED3");
				document.getElementById('LED3_f').src='fan_on.svg';
				console.log("LED3 On");
			}
			else if(message.payloadString==0)
			{
				uncheck("LED3");
				document.getElementById('LED3_f').src='fan_off.svg';
				console.log("LED3 Off");
			}
		}

	else if(message.destinationName=="EEL/client_001/LED4")
		{
			console.log("LED4");
			if(message.payloadString==1)
			{
				check("LED4");
				document.getElementById('LED4_f').src='moon_on.svg';
				console.log("LED4 On");
			}
			else if(message.payloadString==0)
			{
				uncheck("LED4");
				document.getElementById('LED4_f').src='moon_off.svg';
				console.log("LED4 Off");
			}
		}

	else 
		{
			console.log("Some other Message");
		}
	 
 };

//Call to change value of switch and publish
switchValue('#LED1','EEL/client_001/LED1_A');
switchValue('#LED2','EEL/client_001/LED2_A');
switchValue('#LED3','EEL/client_001/LED3_A');
switchValue('#LED4','EEL/client_001/LED4_A');
