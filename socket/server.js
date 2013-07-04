var io = require('socket.io').listen(61001);

io.sockets.on('connection', function (socket) {
	console.log('user connected');
	socket.on('update', function (data) {
		socket.broadcast.emit('client_update', data);
		console.log('Sending data: ' + data);
	});
});