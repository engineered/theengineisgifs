<?php
/**
 * @file rpc.php
 *
 * handles ajax requests
 * steve avery schavery@gmail.com January 2013
 */
require 'inc/common.inc';

if(array_key_exists('more',$_GET))
	print img_list($_GET['after']);