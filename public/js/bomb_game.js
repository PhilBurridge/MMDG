
var btn; 
var hasTheBomb;
var gameOn;

$(document).ready(function() {
	btn = document.getElementById("btn");
	btn.onclick = btnClick;
    initGame();
});

function initGame(){
	hasTheBomb = false;
	gameOn = false;
}

function startGame(){
	gameOn = true;
	setText("La la la...");
	changeBackground("#EEAAAA");
}

function btnClick() {
	if(!gameOn) return;

	if(hasTheBomb){
		throwTheBomb();
	}
	else{
		youLose()
	}
}

function throwTheBomb(){
	hasTheBomb = false;
	changeBackground("#FFEEAA");
	setText("La la la...");
	ws.send("not me!");
}

function catchTheBomb(){
	hasTheBomb = true;
	changeBackground("#FFFF00");
	setText("YOU HAVE THE BOMB!!");
}

function bombExplode(){
	if (hasTheBomb) {
		youLose();
	}
}

function youLose(){
	ws.send("damn it!")
	changeBackground("#FF3300");
	setText("You drink.");
}

function youWin(){
	changeBackground("#00CC00");
	setText("Winning!");
}

function changeBackground(color){
	document.body.style.background = color;
}

function setText(text){
	document.getElementById("text").innerHTML = text;
}