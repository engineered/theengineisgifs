<?php
/*
 * index.php
 * Display the gif.
 * Steve Avery June 2013
 */

require 'inc/common.inc';
?>
<!DOCTYPE html>
<html>
	<head>
		<title><?php echo $page_title; ?></title>
		<link rel="stylesheet" href="css/front.css">
	</head>
	<body>
		<img src='default/default.gif' />
		<?php echo write_socket_client(); ?>
		<script src="js/client_update.js"></script>
	</body>
</html>