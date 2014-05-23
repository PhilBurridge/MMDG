/**
* The class for a Popup window when the user is
* required to enter their name. 
*/
var person="";

function Submitname(d)
{
	this.description = d;
	this.person="";
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
		
		if (person == null || person.length > 12 || person.length < 2 || person == "kuk" || person == "penis" 
		|| person == "fitta" || person == "slida" || person == "bajs" || person == "fuck" || person == "döda" 
		|| person == "neger" || person == "hora" || person == "cp" || person == "jude" || person == "guling" 
		|| person == "nigger" || person == "knark" || person == "sex" || person == "knull" || person == "knulla" 
		|| person == "samlag" || person == "dick" || person == "slut" || person == "whore" || person == "shit" 
		|| person == "nigga" || person == "pussy" || person == "ass" || person == "bitch" || person == "beatch" 
		|| person == "piss" || person == "skit" || person == "död" || person == "snopp" || person == "puta"){
			alert("That nickname is invalid!");
			return false;
		}
		else{
			return true;
		}
	}
}