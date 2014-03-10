var ws = new WebSocket("ws://localhost:1338/");

ws.onopen = function() {
    alert("Opened!");
    ws.send("TJenare! Jag är en client, Detta skrivs från Javascript! Woo!");
};

function buttonDown() {
	

document.getElementById('content').innerHTML += '<br>Button pressed!';
ws.send("Hi server, the button is pressed");


}

function buttonUp(){

document.getElementById('content').innerHTML += '<br>Button released!';
ws.send("Hi server, the button is released");

}