/*
 * client_update.js
 * replaces the gif with the filename provided by the socket.io data
 * Steve Avery June 2013
 */
function client_update(data) {
	document.getElementsByTagName('img')[0].src = data;
}