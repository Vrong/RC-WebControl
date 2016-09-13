
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
		camtime = now;

	if(!camwait || (now - camtime > 500 && camwait) )
	{
		imageServer.requestImage();
		camwait = 1;
		camtime = now;
	}
});


app.listen(8080);

console.log('Webserver online');

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
