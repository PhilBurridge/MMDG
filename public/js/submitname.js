function submitname(d)
{
	this.description = d;
};

Button.prototype = {

	printButton: function(){

		var person = prompt(d,"New Player");
		
		//testa en vector med alla player-namn som kollas genom för att se så att namnet är unikt! LIGGER PÅ APPLIKATIONEN? KANSKE INTE ENS NÖDVÄNDIGT?
		/*if(){
			unique = false;
		}*/
		/*
		if (person == null || person.length > 12 || person.length < 3 || unique == false || person == "kuk" || person == "penis" || person == "fitta" || person == "slida" || person == "bajs" || person == "fuck" || person == "döda" || person == "neger" || person == "hora" || person == "cp" || person == "jude" || person == "guling" || person == "nigger" || person == "knark" || person == "sex" || person == "knull" || person == "knulla" || person == "samlag" || person == "dick" || person == "slut" || person == "whore" || person == "shit" || person == "nigga" || person == "pussy" || person == "ass" || person == "bitch" || person == "beatch" || person == "piss"){
			alert("That name already exists or you have typed the wrong amount of letters!");
			submitname(this.description);
		}*/
		//else{
			ws.send("var=name" config.arg_delimiter + "val=" + person); //skicka personens namn till applikationen
		//}
	}
}