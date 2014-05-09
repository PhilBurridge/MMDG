function Button(n,p,i)
{
	this.name = n;
	this.pos = p;
	this.icon = i;
};

Button.prototype = {

	printButton: function(){
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
}