
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




// ----------------- Video Server Connection

var imageServer = require("./handle_video.js");
var imageServerAddress = '127.0.0.1';
var imageServerPort = 9999;

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

imageServer.connect(imageServerAddress, imageServerPort, imageClientOnDataReceived);

//set resolution
app.post('/setres', function(req, res) {
	imageServer.setResolution(req.body.w, req.body.h);
  res.end();
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
		camtime = now;

	if(!camwait || (now - camtime > 500 && camwait) )
	{
		imageServer.requestImage();
		camwait = 1;
		camtime = now;
	}
});



//-------------controller handling

var controlServer = require("./handle_gamepad.js");
var controlServerAddress = '127.0.0.1';
var controlServerPort = 9998;

var controlClientOnDataReceived = function(data){
}

imageServer.connect(controlServerAddress, controlServerPort, controlClientOnDataReceived);
app.post('/gamepad', function(req, res) {
  controlServer.sendButton(req.body.type, req.body.id, req.body.value);
  res.end();
});

controlServer.connect(controlServerAddress, controlServerPort, controlClientOnDataReceived);

//------------bind server


app.listen(8080);
console.log('Webserver online');
