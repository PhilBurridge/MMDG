
var ws = new WebSocket("ws://" + config.serverIP + ":" + config.serverWsPort + "/");

console.log("Server ip is: %s", config.serverIP);
console.log("Server ws port is: %s", config.serverWsPort);

ws.onopen = function() {
    alert("Opened!");
    ws.send("connected");
    init(ws);
};

function init(ws) {
    ws.onmessage = function (msg) {
	    var data = msg.data;
	    console.log("Message from server: %s", data);

	    if (data === "start") startGame();
	    if (data === "catch") catchTheBomb();
	    if (data === "explode") bombExplode();
	    if (data === "init") initGame();
	    if (data === "you win") youWin();
	    if (data.substring(0,9) === "countdown") countdown(data.substring(10));
	}
}
