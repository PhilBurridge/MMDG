function Gui()
{
	this.buttons = new Array();
	this.displays = new Array();
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
	printEmpty: function(){

		document.write("<div class='empty'></div>");

	}
}