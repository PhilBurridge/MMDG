var ws = new WebSocket("ws://" + config.serverIP + ":" + config.serverWsPort + "/");
var recieved_pings = 0;

console.log("Server ip is: %s", config.serverIP);
console.log("Server ws port is: %s", config.serverWsPort);
console.log("Argument delimiter is: \"%s\"", config.arg_delimiter);

ws.onopen = function() {
    ws.send("var=connected" + config.arg_delimiter + "val=1");
    init(ws);
};


function buttonDown(btnIndex) {
    document.getElementById('content').innerHTML += '<br>Button pressed!';
    ws.send("var=btn" + btnIndex + config.arg_delimiter + "val=1");
}



function buttonUp(btnIndex){
	document.getElementById('content').innerHTML += '<br>Button released!';
	ws.send("var=btn" + btnIndex + config.arg_delimiter + "val=0");
}


function init(ws) {
    ws.onmessage = function (msg) {
	    var data = msg.data;
	    console.log("Message from server: %s",data);
	    if(data.localeCompare("ping")==0){
	    	console.log("Message from server: %s",data);
	    	recieved_pings++;
	    	ws.send("var=recieved_pings" + config.arg_delimiter + "val=" + recieved_pings);
	    }
	    document.getElementById('content').innerHTML += '<br>Message received from server: ' + data;

	    //$('#content').html(+= '<br>Message received from server!')
	    /*$('#state').css({
	        'background-color': data
	    });*/
	}
}