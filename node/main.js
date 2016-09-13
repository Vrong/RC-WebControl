
var express = require('express');
var bodyParser = require('body-parser');
var session = require('express-session');
var request = require('request');


var app = express();

app.use( bodyParser.json() );       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
}));

imgres = [];

app.use( express.static( __dirname + "/public" ) );


app.get('/', function(req, res) {
	sess = req.session;
	console.log('Connecté');
	res.render('index.ejs');
});

var camwait = 0;
var camtime = new Date().getTime();
app.get('/webcam.jpg', function(req, res){
	sess = req.session;
	//console.log('Image requested');
	res.writeHead(200, {'Content-Type': 'image/jpeg'});
	imgres.push(res);
	//console.log(res);
	var now = new Date().getTime();
	if(!camwait)
		camtime = new Date().getTime();

	if(!camwait || (now - camtime > 500 && camwait) )
	{
		imgsocket.write("GET_IMAGE_");
		camwait = 1;
		camtime = now;
	}
});


app.listen(8080);

console.log('Webserver online');

// ----------------- Video Server Connection
var net = require('net');
var fs = require('fs');

var imageServerAddress = '127.0.0.1';
var imageServerPort = 9999;
var imgsocket;

var imageServerConnectionCallback = function(){
  console.log('Connected to image server.');
}

var imageServerConnectionLostCallback = function (){
	console.log('Connection with local client lost. Reconnecting in 3 secondes.');
  setTimeout(imageServerTryConnect, 3000);
}

var imageServerTryConnect = function(){
  imgsocket = new net.Socket();
  imgsocket.setNoDelay(true);
  imgsocket.connect(imageServerPort, imageServerAddress, imageServerConnectionCallback);
  imgsocket.on('close', imageServerConnectionLostCallback);
  imgsocket.on('data', imageClientOnDataReceived);
}

var imageClientOnDataReceived = function(data){
	camwait = 0;
	var sentres = [];
	while(imgres.length != 0)
	{

		imgres[0].end(data);
		imgres.splice(0, 1);
	}
	//fs.writeFile("./test.jpg", data, function(err) { });
}

imageServerTryConnect();


//-------------redimensionner

app.post('/setres', function(req, res) {
	sess = req.session;
	w = req.body.w;
	h = req.body.h;
	// h = req.query.h; //  GET METHOD
	console.log('resolution changed to ' + w + 'x' + h);
	cmd = '';
	while(w.length < 4)
		w = '0' + w;
	while(h.length < 4)
		h = '0' + h;
	cmd = 'SET_RESOLU' + w + h;
	imgsocket.write(cmd);
});

//-------------controller handling
app.post('/gamepad', function(req, res) {
	sess = req.session;
	type = req.body.type;
	index = req.body.id;
	value = req.body.value;
	// h = req.query.h; //  GET METHOD
	console.log('Gamepad request : ' + type + ' ' + index + ' | value: ' + value);
	cmd = '';
  if(type == 'axe')
    type = 'TYP_AXE___';
  else
    type = 'TYP_BUTTON';
	while(index.length < 4)
		index = '0' + index;
  value = value.toString;
  value = value.substring(0, 4);
	while(value.length < 4)
		value = '0' + value;
	cmd = 'GAMEPAD___' + type + index + value;
	//gamepadsocket.write(cmd);

});
