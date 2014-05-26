/** 
* The class for a button, saves the name, position
* and icon address for each button. 
*/
function Button(n,p,i)
{
	this.name = n;
	this.pos = p;
	this.icon = i;
};

Button.prototype = {

    /** 
    * This class' only method, will print the html for a button.
    */
	printButton: function(){

        if( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent)) {

        }
        document.write("<div class='button' style='background-image:url(");
        document.write(this.icon);
        document.write(");' onmousedown='buttonDown(");
        document.write(this.name);
        document.write(");' onmouseup='buttonUp(");
        document.write(this.name);
        document.write(");' ontouchstart='buttonDown(");
        document.write(this.name);
        document.write(")' ontouchend='buttonUp(");
        document.write(this.name);
        document.write(");'></div>");
    }

    // TO DO
    // Implement a way for this class to know if the client wants a 
    // Larger button that takes up 3 fields.
}