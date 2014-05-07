function Display(n,p,i)
{
	this.name = n;
	this.pos = p;
	this.icon = i;
};

Display.prototype = {

	printDisplay: function(){
        document.write("<div class='display' style='background-image:url(");
        document.write(this.icon);
        document.write(");'></div>");
    }
}