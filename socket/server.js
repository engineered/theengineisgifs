var io = require('C:\\Users\\guy-pod\\node_modules\\socket.io').listen(61001);

io.sockets.on('connection', function (socket) {
	console.log('user connected');
	socket.on('update', function (data) {
		socket.broadcast.emit('client_update', data);
		console.log('Sending data: ' + data);
	});
	// socket.on('timednewrequest', function() {
	// 	// get a gif from php
	// 	socket.broadcast.emit('client_update', data);
	// });
});