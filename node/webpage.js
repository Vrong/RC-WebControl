
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
		imgclient.write("GET_IMAGE_");
		camwait = 1;
		camtime = now;
	}
});


app.listen(8080);

console.log('Webserver online');

// ----------------- VIDEO SERVER PART


var net = require('net');
var fs = require('fs');

var imgclient = new net.Socket();
imgclient.setNoDelay(true);
imgclient.connect(9999, '127.0.0.1', function() {
	console.log('Image client connected');
});

imgclient.on('data', function(data) {
	camwait = 0;
	var sentres = [];
	while(imgres.length != 0)
	{
		/*if(sentres.indexOf(imgres[0]) < 0)
		{*/
			imgres[0].end(data);/*
			sentres.push(imgres[0]);
		}
		else {
			console.log("THESAME");
		}*/
		imgres.splice(0, 1);
	}
	//fs.writeFile("./test.jpg", data, function(err) { });

});

imgclient.on('close', function() {
	console.log('Connection closed');
});


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
	imgclient.write(cmd);

	res.end('OK');
});



/*
var net = require('net');
var fs = require('fs');

var nativeVideoClient = new net.Socket();

nativeVideoClient.connect(9999, '127.0.0.1', function() {
	console.log('Connected to native video client');
});
nativeVideoClient.on('data', function(data) {



		udpVideoClient.send(data, udpVideoClient.port, udpVideoClient.address)

});

//------------------------
var PORT = 8888;
var HOST = '127.0.0.1';

var dgram = require('dgram');
var server = dgram.createSocket('udp4');

udpVideo.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', function (message, remote) {
    //console.log(remote.address + ':' + remote.port +' - ' + message);
		udpVideoClient = remote;
		nativeVideoClient.write('message')
});

server.bind(PORT, HOST);
*/
