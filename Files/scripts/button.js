// JavaScript Document
function turnOffButton(button){

if (button.class="active"){
var xmlhttp;
if (window.XMLHttpRequest)
  {// code for IE7+, Firefox, Chrome, Opera, Safari
  xmlhttp=new XMLHttpRequest();
  }
else
  {// code for IE6, IE5
  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
  }
  xmlhttp.open("GET","?pin13=low",true);
xmlhttp.send();
	button.setAttribute("class","inactive");
	button.setAttribute("onClick","turnOnButton(this);");
	button.innerHTML = "OFF";
	return;
}

}
function turnOnButton(button){

if (button.class="inactive"){

var xmlhttp;
if (window.XMLHttpRequest)
  {// code for IE7+, Firefox, Chrome, Opera, Safari
  xmlhttp=new XMLHttpRequest();
  }
else
  {// code for IE6, IE5
  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
  }
  xmlhttp.open("GET","?pin13=high",true);
xmlhttp.send();
	button.setAttribute("class","active");
	button.setAttribute("onClick","turnOffButton(this);");
	button.innerHTML = "ON";
	return;
}

}