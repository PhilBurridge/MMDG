/** 
* The class for a Textfield, saves the name and position
* for each button. 
*/
function Textfield(b,i,p)
{
	this.buttonText = b;
	this.id=i;
	this.pos = p;
};

Textfield.prototype = {

	printTextfield: function(){
        document.write("<div id='textfield'><textarea id ='");
        document.write(this.id);
        document.write("' placeholder='Input text here...'></textarea> <button onclick='buttonClick(");
        document.write(this.id);
        document.write(");'>");
        document.write(this.buttonText);
        document.write("</button> </div>");

    }
}
