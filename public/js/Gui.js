/**
* Main gui class, will create the gui based on given xml-config located in the 
* xml folder. THis class' interface consists of two functions.
* loadElements() will load the xml file and store the information 
* in the Gui objects instance variables.
* drawGui() will write the code for the given buttons and displays stored
* in the object.   
*/
function Gui()
{
	this.buttons = new Array();
	this.displays = new Array();

	/** 
	* An array with the empty fields in the 3x3 grid of divs.
	* When a div is loaded, the array will be emptied respectively.
	* Used in case not every field is defined in the Config file.
	* The empty field must in that case be filled with an empty div.
	*/
	this.empty = [
			"upleft",
			"upcenter",
			"upright",
			"centerleft",
			"centercenter",
			"centerright",
			"downleft",
			"downcenter",
			"downright"
			];
};

// every function inside this prototype can be thought of like the class' methods.
Gui.prototype = {

	loadElements: function(){

		var xmlhttp;
            if (window.XMLHttpRequest){// code for IE7+, Firefox, Chrome, Opera, Safari
              xmlhttp=new XMLHttpRequest();
            }
            else{// code for IE6, IE5
              xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
            }

            xmlhttp.open("GET","xml/config.xml",false);
            xmlhttp.send();
            xmlDoc=xmlhttp.responseXML;

            var b = xmlDoc.getElementsByTagName("BUTTON"); //Buttons
            var d = xmlDoc.getElementsByTagName("DISPLAY"); //Buttons

	        for (var i=0;i<b.length;i++){

	         	var pos = b[i].getElementsByTagName("POS")[0].childNodes[0].nodeValue; 
	            var name = b[i].getElementsByTagName("NAME")[0].childNodes[0].nodeValue;
	            var icon = b[i].getElementsByTagName("ICON")[0].childNodes[0].nodeValue;
	         	this.buttons.push(new Button(name,pos,icon));
	         	this.remove(pos);
	        }

	        for (var i=0;i<d.length;i++){

	         	var pos = d[i].getElementsByTagName("POS")[0].childNodes[0].nodeValue; 
	            var name = d[i].getElementsByTagName("NAME")[0].childNodes[0].nodeValue;
	            var icon = d[i].getElementsByTagName("ICON")[0].childNodes[0].nodeValue;

	         	this.displays.push(new Display(name,pos,icon));
	         	this.remove(pos);
	        }

	},

	/** 
	* If a position in the Gui has been filled with an element defined in the
	* XML config file: Then that same position needs to be removed from the
	* empty-list. if the position up, down, right or left has been filled, 
	* then all the fields on that side needs to be removed from the empty list.
	* (This function exists in case you want to fill a whole row with 1 button or display) 
	*/
	remove: function(pos){

		var i = this.empty.indexOf(pos);
		if(i != -1) {
			this.empty.splice(i, 1);

			if(pos=="up"){
				var l = this.empty.indexOf("upleft");
					if(l != -1) 
						this.empty.splice(l, 1);
				var c = this.empty.indexOf("upcenter");
					if(c != -1) 
						this.empty.splice(c, 1);
				var r = this.empty.indexOf("upright");
					if(r != -1) 
						this.empty.splice(r, 1);

			}

			else if(pos=="right"){

				var u = this.empty.indexOf("upright");
					if(u != -1) 
						this.empty.splice(u, 1);
				var r = this.empty.indexOf("centerright");
					if(r != -1) 
						this.empty.splice(r, 1);
				var d = this.empty.indexOf("downright");
					if(d != -1) 
						this.empty.splice(d, 1);

			}

			else if(pos=="left"){

				var u = this.empty.indexOf("upleft");
					if(u != -1) 
						this.empty.splice(u, 1);
				var r = this.empty.indexOf("centerleft");
					if(r != -1) 
						this.empty.splice(r, 1);
				var d = this.empty.indexOf("downleft");
					if(d != -1) 
						this.empty.splice(d, 1);

			}
			else if(pos=="down"){

				var l = this.empty.indexOf("downleft");
					if(l != -1) 
						this.empty.splice(l, 1);
				var c = this.empty.indexOf("downcenter");
					if(c != -1) 
						this.empty.splice(c, 1);
				var r = this.empty.indexOf("downright");
					if(r != -1) 
						this.empty.splice(r, 1);

			}
		}
	},

	/** 
	* Pretty primitive way to build the layout. the elements need to
	* be drawn in a certain order, otherwise they will be mixed up.
	* Might need to be refactored in the future.
	*/
	drawGui: function(){

		this.drawElement("up");
		this.drawElement("left");
		this.drawElement("upleft");
		this.drawElement("upcenter");
		this.drawElement("upright");

		this.drawElement("centerleft");
		this.drawElement("centercenter");
		this.drawElement("centerright");
		this.drawElement("down");

		this.drawElement("downleft");
		this.drawElement("downcenter");
		this.drawElement("downright");
		this.drawElement("right");


	},

	/** 
	* Will call the elements draw methods.
	*/
	drawElement: function(ori){

		for(var i=0;i<this.buttons.length;i++){
			if(this.buttons[i].pos == ori)
				this.buttons[i].printButton();	
		}
		for(var i=0;i<this.displays.length;i++){
			if(this.displays[i].pos == ori)
				this.displays[i].printDisplay();			
		}
		for(var i=0;i<this.empty.length;i++){
			if(this.empty[i] == ori)
				this.printEmpty();

				
		}
	},

	/** 
	* will write the code for an empty div that fills one field.
	*/
	printEmpty: function(){

		document.write("<div class='empty'></div>");

	}
}