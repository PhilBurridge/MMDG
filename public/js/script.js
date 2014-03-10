var ws = new WebSocket("ws://130.236.124.32:1338/");

ws.onopen = function() {
    alert("Opened!");
    ws.send("TJenare! I am a client, This is from JS!");
    init(ws);
};

function buttonDown() {


	document.getElementById('content').innerHTML += '<br>Button pressed!';
	ws.send("Hi server, the button is pressed");


}

function buttonUp(){

	document.getElementById('content').innerHTML += '<br>Button released!';
	ws.send("Hi server, the button is released");

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