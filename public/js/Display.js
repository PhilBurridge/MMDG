/** 
* The class for a Display, saves the name, position
* and icon address for each display. 
*/
function Display(n,p,i)
{
	this.name = n;
	this.pos = p;
	this.icon = i;
};

Display.prototype = {

	printDisplay: function(){
        document.write("<div id='display' style='background-image:url(");
        document.write(this.icon);
        document.write(");'></div>");
    }
}