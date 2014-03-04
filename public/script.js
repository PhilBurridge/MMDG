var ws = new WebSocket("ws://127.0.0.1:1338/");

ws.onopen = function() {
    alert("Opened!");
    ws.send("Hello Server");
};

function buttonDown() {
	

document.getElementById('content').innerHTML += '<br>Button pressed!';
ws.send("Hi server, the button is pressed");


}

function buttonUp(){

document.getElementById('content').innerHTML += '<br>Button released!';
ws.send("Hi server, the button is released");

}