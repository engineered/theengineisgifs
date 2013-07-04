<?php
/**
 * @file settings.php
 * 
 * set configurations here
 */
/*
 * Page title to display on the admin page
 */
$page_title = 'A Machine for Gifs';

/*
 * the url of the host where the gif will be displayed
 */
$base_url = 'http://theengineis.gifs'; // no trailing slash

/*
 * mysql db information
 */
$db_info = array(
	'user' => 'gifmaster',
	'password' => 'Dt4qWpVcYEE6wB2y',
	'host' => 'localhost',
	'port' => 3306,
	'db_name' => 'gifs',
);

/*
 * socket.io information
 */
$socket_info = array(
	'host' => 'localhost',
	'port' => 61001,
);

/*
 * adds a cronjob for updating to the next gif in the queue automatically
 * not implemented.
 */
$cron_info = array(
	'enabled' => 0,
	'time' => '0600',
);

/*
 * twitter integration
 * not implemented.
 */
$twitter_info = array();