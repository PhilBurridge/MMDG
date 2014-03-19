
var ws = new WebSocket("ws://" + config.serverIP + ":" + config.serverWsPort + "/");

console.log("Server ip is: %s", config.serverIP);
console.log("Server ws port is: %s", config.serverWsPort);

ws.onopen = function() {
    alert("Opened!");
    ws.send("TJenare! I am a client, This is from JS!");
    init(ws);
};

// Returns value=1 to App on click
function buttonDownVal1() {
	document.getElementById('content').innerHTML += '<br>Button pressed!';
	ws.send("value=1");
}

// Returns value=0 to App on click
function buttonDownVal0() {
    document.getElementById('content').innerHTML += '<br>Button pressed!';
    ws.send("value=0");
}

function buttonUp(){

	document.getElementById('content').innerHTML += '<br>Button released!';
	ws.send("value=0");

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