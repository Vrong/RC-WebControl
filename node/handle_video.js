var net = require('net');
var fs = require('fs');
var imgsocket;
var connected = false;
var address = "127.0.0.1";
var port = "9999";
var begin = false;
var debug = 0;
var onDataCallback;

var requestImage = function()
{
		imgsocket.write("GET_IMAGE_");
}

var setResolution = function(h, w)
{
  if(debug > 0) console.log('resolution changed to ' + w + 'x' + h);
  cmd = '';
  while(w.length < 4)
    w = '0' + w;
  while(h.length < 4)
    h = '0' + h;
  cmd = 'SET_RESOLU' + w + h;
  imgsocket.write(cmd);
}

var isConnected = function()
{
  return connected;
}

var connect = function(addr, prt, onData)
{
  address = addr;
  port = prt;
  onDataCallback = onData;
  if(begin == false)
  {
    begin = true;
    imageServerTryConnect();
  }
}


var imageServerConnectionCallback = function(){
  if(debug > 0) console.log('Connected to image server.');
  connected = true;
}

var imageServerConnectionLostCallback = function (){
  connected = false;
	if(debug > 0) console.log('Connection with local client lost. Reconnecting in 3 secondes.');
  setTimeout(imageServerTryConnect, 3000);
}

var imageServerTryConnect = function(){
	if(debug > 0) console.log('Connecting to video server...');
  imgsocket = new net.Socket();
  imgsocket.setNoDelay(true);
  imgsocket.connect(port, address, imageServerConnectionCallback);
  imgsocket.on('close', imageServerConnectionLostCallback);
  imgsocket.on('data', onDataCallback);
}

module.exports = {
  connect: connect,
  isConnected: isConnected,
  requestImage: requestImage,
  setResolution: setResolution,
  debug: debug
};
