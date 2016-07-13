var net = require('net');
var fs = require('fs');

var client = new net.Socket();
client.connect(9999, '127.0.0.1', function() {
	console.log('Connected');
});

client.on('data', function(data) {
	
	//console.log('Received: ' + data + ' \n');


	fs.writeFile("./test.jpg", data, function(err) {
    if(err) {
        return console.log(err);
    }

    console.log("The file was saved!");
}); 

});

client.on('close', function() {
	console.log('Connection closed');
});

client.write('Hello, server! Love, Client.');


