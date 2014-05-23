/**
* The class for a Popup window when the user is
* required to enter their name. 
*/
var person="";

function Submitname(d)
{
	this.description = d;
	this.person="";

	badWords = ["kuk","penis","fitta","slida",
				"bajs","fuck","döda","neger",
				"hora","cp","jude","guling",
				"nigger","knark","sex","knull",
				"knulla","samlag","dick","slut",
				"whore","shit","nigga","pussy",
				"ass","bitch","beatch","piss",
				"skit","död","snopp","puta", 
				"kukpenis", "hitler", "kukfitta",
				"fitthitler", "adolf", "fittler"];
};

Submitname.prototype = {

	printSubmitname: function(){
		
		//A popup window that needs to be answered in order to connect to the web page and application
		person = prompt(this.description,"New Player");
		
		//var unique = true;
		//testa en vector med alla player-namn som kollas genom för att se så att namnet är unikt! LIGGER PÅ APPLIKATIONEN? KANSKE INTE ENS NÖDVÄNDIGT?
		/*if(){
			unique = false;
		}*/
		
		if (person == null || person.length > 12 || person.length < 2){
			alert("Nickname must have 2 - 12 characters.");
			return false;
		}

		for (var i = 0; i < badWords.length; i++) {
			if (person.toLowerCase() === badWords[i]){
				alert("That nickname is invalid!");
				return false;
			}
		};

		return true;
	}	
}