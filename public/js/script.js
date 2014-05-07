var ws = new WebSocket("ws://" + config.serverIP + ":" + config.serverWsPort + "/");
var recieved_pings = 0;

console.log("Server ip is: %s", config.serverIP);
console.log("Server ws port is: %s", config.serverWsPort);
console.log("Argument delimiter is: \"%s\"", config.arg_delimiter);

ws.onopen = function() {
	var device = "computer";
	if( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent)) {
 		device = "mobile";
	}
    ws.send("var=connected" + config.arg_delimiter + "val=" + device);
    init(ws);
};

function buttonDown(btnId) {
	//document.getElementById('display').innerHTML += '<br>pushed! ';
	postOnDisplay("pushed!!");
    ws.send("var=btn" + btnId + config.arg_delimiter + "val=1");
};

function buttonUp(btnId){
	//document.getElementById('display').innerHTML += '<br>released! ';
	postOnDisplay("released!");
	ws.send("var=btn" + btnId + config.arg_delimiter + "val=0");
};

function init(ws) {
    ws.onmessage = function (msg) {
	    var data = msg.data;
	    console.log("Message from server: %s",data);
	    if(data.localeCompare("ping")==0){
	    	console.log("Message from server: %s",data);
	    	recieved_pings++;
	    	ws.send("var=recieved_pings" + config.arg_delimiter + "val=" + recieved_pings);
	    }
	    postOnDisplay(data);
	}
};

function postOnDisplay(message){

	var d = document.getElementById('display');
	d.innerHTML += "<br>" + message;

	if(d.scrollHeight > d.clientHeight) {
  		d.scrollTop = d.scrollHeight - d.clientHeight;
	}
}

