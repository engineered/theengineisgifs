<?php
/**
 * @file common.inc
 *
 * includes function definitions for gifs
 * Steve Avery steve@theengineisred.com September 2012
 */

//error_reporting(E_ALL);

/* form for submitting images, used on admin.php */
$form = <<<EOT
	<form method="post" action="admin.php">
		<table>
			<tbody>
				<tr>
					<td><label for="url">Post a new .gif:</label></td>
				</tr>
				<tr class="control-group warning">
					<td><i class="icon-chevron-right icon-white"></i><input type="text" name="url" id="url" placeholder="URL" /></td>
				</tr>
				<tr>
					<td><label for="title">Title:</label></td>
				</tr>
				<tr class="control-group warning">
					<td><i class="icon-chevron-right icon-white"></i><input type="text" placeholder="The Engine is ..." name="title" id="title" /></td>
				</tr>
				<tr>
					<td class="centered"><input class="btn btn-inverse submission" type="submit" value="Queue" /></td>
				</tr>
			</tbody>
		</table>
	</form>
EOT;

require 'conf/settings2.php';
function db_connect() {
	global $db_info;

	if ($db_info['port'] != 3306) { // mysql default
		$host = $db_info['host'] . ':' . $db_info['port'];
	} else {
		$host = $db_info['host'];
	}

	try {
		$db = mysql_connect($host, $db_info['user'], $db_info['password']);
		mysql_select_db($db_info['db_name']);
	} catch (Exception $e) {
		// Pretty sure I do not know what I am doing here
		echo 'Couldn\'t connect to db. Check your db configurations.\n';
		var_dump($e->getMessage());
		exit();
	}
	return $db;
}


/*
 * receives post request from admin.php
 * if the post request has the proper keys (title and url)
 * then we clean the data and submit it to the database
 * also figures out about when a gif would be deployed
 */
function post($url, $title) {
	$db = db_connect();

	$url = mysql_real_escape_string(trim($url));
	$title = mysql_real_escape_string(trim($title));

	$sql = "INSERT INTO `history` (`submitted`,`title`,`url`) VALUES (UNIX_TIMESTAMP(),'" . $title . "','" . $url . "')";
	mysql_query($sql);

	// clean url once more for display on the page.
	$url = htmlentities($url);
	mysql_close();
}

/*
 * updates the markup in index.html
 * updates the db with the time that the current filename was run
 */
function set_next() {
	/*
	 * updates img tag in index.html
	 * sets runtime for that url
	 */
	$db = db_connect();
	
	$sql = 'SELECT `file` FROM `history` WHERE `displayed` IS NULL AND `removed` != 1 ORDER BY `submitted` ASC LIMIT 1';
	$resource = mysql_query($sql);
	if(mysql_num_rows($resource)) {
		$file = mysql_result($resource, 0, 0);
	} else {
		echo 'no results to show';
		mysql_close();
		return;
	}

	file_put_contents('../index.html',html($file));

	// set time.
	$sql = "UPDATE `history` SET `displayed` = UNIX_TIMESTAMP() WHERE `file` = '$file'";
	mysql_query($sql);

	mysql_close();
}

/*
 * finds urls to download and save, updates the db with filenames
 */
function dl_new_imgs() {
	ini_set('max_execution_time', 300);
	$db = db_connect();

	// first, find the highest filename, so we can name further files.
	$sql = 'SELECT `file` FROM `history` WHERE `file` IS NOT NULL ORDER BY ABS(`file`) DESC LIMIT 1';
	$resource = mysql_query($sql);
	if(mysql_num_rows($resource)) {
		$filename = mysql_result($resource, 0, 0);
		$filecount = substr($filename, 0, -4); // remove '.gif'
	} else {
		// there are either none to dl or none with names.
		$sql = 'SELECT COUNT(*) FROM `history` WHERE `file` IS NULL';
		$resource = mysql_query($sql);
		if(mysql_num_rows($resource)) {
			$filecount = 0;
		} else {
			mysql_close();
			echo 'none to download';
		}
	}

	// download any files that dont have a file name.
	$sql = "SELECT `url` FROM `history` WHERE `file` IS NULL";
	$resource = mysql_query($sql);
	$updates = array();
	while($row = mysql_fetch_assoc($resource)) {
		$filecount++;
		$filename = $filecount . '.gif';

		// just realized that theres a lot of potential insecurity here. boo.
		$ch = curl_init($row['url']);
		$fp = fopen("C:\\www\\vhosts\\theengineisgifs\\archive\\$filename", 'wb');
		curl_setopt($ch, CURLOPT_FILE, $fp);
		curl_setopt($ch, CURLOPT_HEADER, 0);
		curl_exec($ch);
		curl_close($ch);
		fclose($fp);

		$updates[$row['url']] = $filename;

		// get the frame for preview
		$image = new Imagick("C:\\www\\vhosts\\theengineisgifs\\archive\\$filename");
		$coalesced = $image->coalesceImages();

		foreach ($coalesced as $frame) {
			$framename = "C:\\www\\vhosts\\theengineisgifs\\archive\\frames\\$filename";
			$frame->writeImage($framename);
		break;
		}
	} // end while

	// create update statement for filenames.
	if(count($updates)) {
		foreach($updates as $url => $filename) {
			$sql = "UPDATE `history` SET `file` = '$filename' WHERE `url` = '$url';";
			mysql_query($sql);
		}
	}
	mysql_close();

}

/* slated for removal in favor of new style */
function history($offset = 0) {
	// get previews of all gifs
	$db = db_connect();

	$sql = 'SELECT `submitted`,`displayed`,`file` FROM `history` WHERE `file` IS NOT NULL AND `removed` != 1 ORDER BY `submitted` DESC LIMIT 20 OFFSET ' . $offset;
	
	$resource = mysql_query($sql);
	if(mysql_num_rows($resource)) {
		if(mysql_num_rows($resource) < 20) { // less than the limit
			// here we will need to append an end of list flag
		} else {
			$rows = array();
			while($row = mysql_fetch_assoc($resource)) {
				$rows[] = $row;
			}
		}
	} else { // isnt always right, now that pagination is real
		$history = '<tr class="default"><td><a href="default/default.gif" target="_blank"><img class="tabled" src="default/default_frame.gif" /></a></td><td>--</td><td>--</td></tr>';
	}
	mysql_close();

	if(!isset($history)) {
		$history = '';
		foreach($rows as $row) {
			$history .= '<tr>';
			$frame = '/archive/frames/' . $row['file'];
			$file = '/archive/' . $row['file'];
			$history .= '<td><a href="' . $file . '" target="_blank"><img class="tabled" src="' . $frame . '" /></a></td>';

			if($row['displayed']) { // format ze date
				$date = new DateTime();
				$date->setTimestamp($row['displayed']);
				$date->setTimezone(new DateTimeZone('America/Los_Angeles'));
				$date = $date->format('F j, Y, g:i a');
				$history .= '<td><span class="date">' . $date . '</span></td>';
			} else {
				$history .= '<td><span class="undisplayed">--</span></td>';
			}
			
			$history .= '<td><a class="btn btn-inverse show_this" href="javascript:void(0)">Display Now</a><a class="btn btn-danger delete_this" href="javascript:void(0)">Delete</a></td>';
			$history .= '</tr>' . "\n";
		}
	}
	
	return $history;
}

// marks a row as removed
function remove($filename) {
	$db = db_connect();
	$sql = 'UPDATE `history` SET `removed` = 1 WHERE `file` = \'' . $filename . '\'';
	mysql_query($sql);
	mysql_close();
}

function history_table() {
	$history = history();
	$table = <<<EOL
		<div id="controls">
			<a href="javascript:void(0)" id="back" class="btn btn-inverse">Go Back</a>
			<a href="javascript:void(0)" id="cron" class="btn btn-inverse">Update Database</a>
		</div>
		<div id="history">
			<table class="table table-hover">
				<thead>
					<tr id="sticky">
						<th>Image</th>
						<th>Displayed</th>
						<th>Actions</th>
					</tr>
				</thead>
				<tbody>
					$history
				</tbody>
			</table>
		</div>
EOL;
	return $table;
}

/*
 * emit sends notice to node server 
 */
function emit($filename) {
	require '/lib/Client.php';
	//echo 'in emit';

	$elephant = new ElephantIO\Client('http://theengineis.gifs:61001', 'socket.io', 1, false, true, true);
	$elephant->init();
	$elephant->send(
		ElephantIO\Client::TYPE_EVENT,
		null,
		null,
		json_encode(array('name' => 'update', 'args' => $filename))
	);
	$elephant->close();
}

/*
 * img_list
 */
function img_list($offset = 0) {
	$list_to_return = array(
		'end_of_list' => 0,
		'html_string' => ''
	);
	$db = db_connect();
	$sql = 'SELECT `submitted`,`displayed`,`file` FROM `history` WHERE `file` IS NOT NULL AND `removed` != 1 ORDER BY `submitted` DESC LIMIT 20 OFFSET ' . $offset;
	$resource = mysql_query($sql);
	if(mysql_num_rows($resource)) {
		if(mysql_num_rows($resource) < 20) { 
			$list_to_return['end_of_list'] = 1;
		} else {
			$rows = array();
			while($row = mysql_fetch_assoc($resource)) {
				$rows[] = $row;
			}
		}
	} else {
		// there were no db results. maybe your confs are wrong,
		// or you're just getting started!
		$list_to_return['end_of_list'] = 1;
		$img_list = '<div class="frame default filled"><div class="inner"><a href="default/default.gif" target="_blank"><img src="default/default_frame.gif" /></a></div><div class="controls btn-group"><button class="btn btn-warn btn-disabled">Display</button><button class="btn btn-disabled">Remove</button><button class="btn btn-disabled">Tweet</button></div></div>';
	}
	mysql_close();

	// only enter this block if there were results from the db
	if(!isset($img_list)) {
		$img_list = '';
		foreach($rows as $row) {
			$img_list .= '<div class="frame filled"><div class="inner">';
			$frame = 'archive/frames/' . $row['file'];
			$file = 'archive/' . $row['file'];
			$img_list .= '<a href="' . $file . '" target="_blank"><img src="' . $frame . '" /></a></div>';
			if($row['displayed']) {
				$img_list .= '<div class="controls btn-group"><button class="btn btn-info">Display</button><button class="btn btn-warn">Remove</button><button class="btn">Tweet</button></div></div>';
			} else {
				$img_list .= '<div class="controls btn-group"><button class="btn">Display</button><button class="btn btn-warn">Remove</button><button class="btn">Tweet</button></div></div>';
			}
		}
	}
	$list_to_return['html_string'] = $img_list;
	return json_encode($list_to_return);
}


/*
 * get distinct filenames from history
 */
function filelist() {
	$db = db_connect();
	$sql = "SELECT DISTINCT file FROM `history`";
	$resource = mysql_query($sql);
	$result = array();
	if(mysql_num_rows($resource)) {
		while($row = mysql_fetch_assoc($resource)) {
			$result[] = $row['file'];
		}
	}
	return json_encode($result);
}

/*
 * return a string to write in the index.php file, for the socket.io client code.
 */
function write_socket_client() {
	return "<script src=\"" . $socket_info['host'] . ':' . $socket_info['port'] . "/socket.io/socket.io.js\"></script>";
}

