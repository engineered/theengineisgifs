<?php
/**
 * @file admin.php
 *
 * administer and record gifs for display on the tv
 * in the front of the office.
 *
 * Steve Avery steve@theengineisred.com August 2012
 */ 

require('inc/common.inc');
if(array_key_exists('filelist', $_GET)) {
	print filelist();
	die();
}
?>
<!DOCTYPE html>
<html>
	<head>
		<title>Engine .gifs</title>
		<link rel="stylesheet" href="css/bootstrap.min.css" />
		<link rel="stylesheet" href="css/main.css" />
		<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
		<script src="js/hoverIntent.js"></script>
		<script src="js/dateFormat.js"></script>
	</head>
	<body>
<?php if($_POST): ?>
	<?php if(($url = $_POST['url']) && ($title = $_POST['title'])): ?>
		<?php post($url, $title); ?>
		<!-- successful -->
		<div id="success">
			<h1>Success!</h1>
			<img src="<?php echo $url; ?>" />
		</div>
	<?php else: ?>
		<!-- empty submission -->
		<div id="tryagain">
			There was an error in your submission. Please try again!
		</div>
		<?php print($form); ?>
	<?php endif; ?>
<?php elseif($_GET): ?>
	<?php if(array_key_exists('delete',$_GET)): ?>
		<?php print remove($_GET['file']); ?>
	<?php elseif(array_key_exists('async_display',$_GET)): ?>
		<?php emit($_GET['file']); ?>
	<?php elseif(array_key_exists('more',$_GET)): ?>
		<?php print img_list($_GET['after']); ?>
	<?php elseif(array_key_exists('update',$_GET)): ?>
		<?php dl_new_imgs(); ?>
		<?php print history_table(); ?>
	<?php elseif(array_key_exists('history',$_GET)): ?>
		<?php print history_table(); ?>
	<?php endif; ?>
<?php else: ?>
	<!-- submit a new gif -->
	<?php print($form); ?>
	<a class="btn" id="history_btn" href="javascript:void(0)">See queue</a> 
<?php endif; ?>
		<script src="js/main.js"></script>
	</body>
</html>