<?php
/**
 * @file post.php
 *
 * The unified admin interface.
 * Steve Avery steve@theengineisred.com January 2013
 */

require 'inc/common.inc';
?>
<!DOCTYPE html>
<html class="version1">
	<head>
		<title>Gif Machine - Admin</title>
		<link rel="stylesheet" href="css/bootstrap.min.css" />
		<link rel="stylesheet" href="css/main.css" />
		<script src="//ajax.googleapis.com/ajax/libs/jquery/2.0.2/jquery.min.js"></script>
		<script src="js/hoverIntent.js"></script>
		<script src="js/dateFormat.js"></script>
		<script src="js/jquery.masonry.min.js"></script>
		<!--<script src="js/jquery.masonry.cornerstamp.min.js"></script>-->
	</head>
	<body>
		<header>
			<div class="navbar">
				<h2 class="inline"><?php print $page_title; ?></h2>
			</div>
		</header>
		<div role="main" class="main">
			<div id="loading">
				<p>Hang on a second...</p>
			</div>
			<div class="list">
				<div class="blank blank-form frame">
					<div class="inner">
						<form class="new-submission">
							<table>
								<tr>
									<td>
										<label for="url-field">Paste the URL of the image here:</label>
									</td>
								</tr>
								<tr>
									<td>
										<input type="url" name="url" id="url-field" autofocus required pattern="^.*\.(gif|GIF)$" placeholder="Image URL" />
									</td>
								</tr>
								<tr>
									<td>
										<label for="title-field">Have a clever title for your image?</label>
									</td>
								</tr>
								<tr>
									<td>
										<input type="text" name="title" id="title-field" placeholder="Compose new title..." />
									</td>
								</tr>
								<tr>
									<td>
										<button class="btn btn-inverse" id="go-new-submission">Add it</button>
									</td>
								</tr>
							</table>
						</form>
					</div>
				</div>
			</div>
		</div>
		<script src="js/main.js"></script>
	</body>
</html>