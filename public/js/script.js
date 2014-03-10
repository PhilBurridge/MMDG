
var ws = new WebSocket("ws://" + config.serverIP + ":" + config.serverWsPort + "/");

console.log("Server ip is: %s", config.serverIP);
console.log("Server ws port is: %s", config.serverWsPort);

ws.onopen = function() {
    alert("Opened!");
    ws.send("TJenare! I am a client, This is from JS!");
    init(ws);
};

function buttonDown() {
	document.getElementById('content').innerHTML += '<br>Button pressed!';
	ws.send("button pressed");
}

function buttonUp(){

	document.getElementById('content').innerHTML += '<br>Button released!';
	ws.send("button released");

}


function init(ws) {
    ws.onmessage = function (msg) {
	    var data = msg.data;
	    console.log("Message from server: %s",data);
	    document.getElementById('content').innerHTML += '<br>Message received from server: ' + data;

	    //$('#content').html(+= '<br>Message received from server!')
	    /*$('#state').css({
	        'background-color': data
	    });*/
	}
}