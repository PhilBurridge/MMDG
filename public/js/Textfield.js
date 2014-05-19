function Textfield(n,p,i)
{
	this.name = n;
	this.pos = p;
};

Textfield.prototype = {

	printTextfield: function(){
        document.write("<div id='textfield'><textarea placeholder='Input text here...' name='");
        document.write(name);
        document.write("'></textarea> </div>");
//rows='35' cols='90'
    }
}
