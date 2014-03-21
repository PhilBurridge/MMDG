
var btn; 
var hasTheBomb;
var gameOn;

$(document).ready(function() {
	btn = document.getElementById("btn");
	btn.onclick = btnClick;
    hasTheBomb = false;
    gameOn = false;
});

function startGame(){
	console.log("Start game");
	hasTheBomb = false;
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
		youLose();
	}
}

function throwTheBomb(){
	hasTheBomb = false;
	printBomb()
	changeBackground("#FFEEAA");
	setText("La la la...");
	ws.send("not me!");
}

function catchTheBomb(){
	hasTheBomb = true;
	printBomb()
	changeBackground("#FFFF00");
	setText("YOU HAVE THE BOMB!!");
}

function bombExplode(){
	ws.send("hasTheBomb=" + hasTheBomb);
	if (hasTheBomb === true) {
		console.log("you exploded");
		youLose();
		return;
	}
	youWin();
}

function youLose(){
	console.log("you lose");
	changeBackground("#FF3300");
	setText("You drink.");
	ws.send("damn it");
	gameOn = false;
}

function youWin(){
	changeBackground("#00CC00");
	setText("Winning!");
}

function countdown(seconds){
	gameOn = false;
	var counter = setInterval(function(){
		seconds--;
		setText(seconds);
		if(seconds <= 1){
			clearInterval(counter);
		}
	}, 1000);
}

function printBomb(){
	console.log("hasTheBomb=" + hasTheBomb);
}

function changeBackground(color){
	document.body.style.background = color;
}

function setText(text){
	document.getElementById("text").innerHTML = text;
}