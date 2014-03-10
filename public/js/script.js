var ws = new WebSocket("ws://130.236.124.32:1338/");

ws.onopen = function() {
    alert("Opened!");
    ws.send("TJenare! I am a client, This is from JS!");
};

function buttonDown() {
	

document.getElementById('content').innerHTML += '<br>Button pressed!';
ws.send("Hi server, the button is pressed");


}

function buttonUp(){

document.getElementById('content').innerHTML += '<br>Button released!';
ws.send("Hi server, the button is released");

}