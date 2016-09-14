var net = require('net');
var fs = require('fs');
var gamepadsocket;
var connected = false;
var address = "127.0.0.1";
var port = "9998";
var begin = false;
var debug = 0;
var onDataCallback;


var sendButton = function(type, index, value)
{
	if(debug > 0) cconsole.log('Gamepad request : ' + type + ' ' + index + ' | value: ' + value);
	cmd = '';
  if(type == 'axe')
    type = 'TYP_AXE___';
  else
    type = 'TYP_BUTTON';
	while(index.length < 4)
		index = '0' + index;
  value = value.toString().substring(0, 6);
	while(value.length < 6)
		value = value + ' ';
	cmd = 'GAMEPAD___' + type + index + value;
	gamepadsocket.write(cmd);

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
    controlServerTryConnect();
  }
}

var controlServerConnectionCallback = function(){
  if(debug > 0) console.log('Connected to control server.');
  connected = true;
}

var controlServerConnectionLostCallback = function (){
  connected = false;
	if(debug > 0) console.log('Connection with local client lost. Reconnecting in 3 secondes.');
  setTimeout(controlServerTryConnect, 3000);
}

var controlServerTryConnect = function(){
  if(debug > 0) console.log('Connecting to control server...');
  gamepadsocket = new net.Socket();
  gamepadsocket.setNoDelay(true);
  gamepadsocket.connect(port, address, controlServerConnectionCallback);
  gamepadsocket.on('close', controlServerConnectionLostCallback);
  gamepadsocket.on('data', onDataCallback);
}

module.exports = {
  connect: connect,
  sendButton: sendButton,
  debug: debug
};
