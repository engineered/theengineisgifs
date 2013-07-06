(function($){
	var gifs = gifs || {};
	// slated for removal in new version
	$('.submission').click(function(e) {
		if($('#url').val().split('.').pop() !== 'gif') {
			e.preventDefault();
			alert('Please sumbit a gif');
		}
	});

	// slated for removal in new version
	$('#history_btn').click(function() {
		$.ajax({
			url:'admin.php',
			data:{
				history:1,
			},
		}).done(function( data ) {
			$('body').html(data);
			window.location.href = 'admin.php?history';
		});
	});
	
	// slated for removal in new version
	$('.delete_this').click(function() {
		gifs.row = $(this).parents('tr');
		gifs.filename = row.find('img').attr('src').substring(9);
		$.ajax({
			url:'admin.php',
			data:{
				delete:1,
				file:filename,
			},
		}).done(function() {
			row.hide();
		});
	});

	// slated for removal in new version
	$('.show_this').click(function() {
		gifs.row = $(this).parents('tr');
		gifs.filename = row.find('img').attr('src').substring(9);
		$.ajax({
			url:'admin.php',
			data:{
				async_display:1,
				file:gifs.filename,
			},
		}).done(function() {
			gifs.date = new Date();
			row.find('.date').html(gifs.date.format('mmmm d, yyyy, h:MM TT'));
			row.addClass('success');
			setTimeout(function() {
				gifs.row.removeClass('success');
			},250);
		});
	});

	// hoverconfig for hoverintent bindings
	gifs.hoverConfig = {
		over: function() {
			gifs.still = $(this).find('img');
			if($(this).hasClass('default')) {
				gifs.mover = 'default/default.gif';
			} else {
				gifs.mover = 'archive/' + gifs.still.attr('src').split('/').pop();
			}
			gifs.still.attr('src',gifs.mover);
		},
		out: function() {
			gifs.mover = $(this).find('img');
			if($(this).hasClass('default')) {
				gifs.still = 'default/default_frame.gif';
			} else {
				gifs.still = 'archive/frames/' + gifs.mover.attr('src').split('/').pop();
			}
			gifs.mover.attr('src',gifs.still);
		},
		sensitivity: 10,
	};
	//$('tbody tr').hoverIntent(hoverConfig);

	// slated for removal in new version
	$('#back').click(function() {
		window.location.href = 'admin.php';
	});

	// slated for removal in new version
	$('#cron').click(function() {
		// ajax to run update db
		$.ajax({
			url:'admin.php',
			data:{
				update:1,
			},
		}).done(function( data ) {
			$('body').html(data);
		});
	});

	// handle submission from version 1
	$('#go-new-submission').click(function(e) {
		e.preventDefault();
		$('#loading').show();
		gifs.url = $('#url-field').val();
		gifs.title = $('title-field').val();

		if(title !== '') {
			gifs.dataObject.url = url;
			gifs.dataObject.title = title;
		} else {
			gifs.dataObject.url = url;
		}

		// reflow and update the page
		$('.blank-form').fadeTo(400, 0, function() {
			$('.blank-form').html(formMarkup);

			//$('.blank-form').fadeTo(100, 1);
		});
		
		// slide down
		// load in a new blank frame


		$.ajax({
			url: 'rpc.php',
			data: gifs.dataObject,
			success: function(data) {
				// hide loading
				// draw in the blank frame
			},
			error: function() {
				// hide the loading
				// hide the new image slot
				// slide up
			}
		});
	});
/*
	// handle reflow list animation
	var listreflow = function(index = 0, direction = 'down') {
		// if down
		// iterate through each frame from the bottom of the list to the top
		// animate it over by the right amount of pixels and if its the last
		// position	in a row animate it diagonally

		// if up
		// start at top and go left
		// unless its the first in a row
		// then go diagonally to last position

		// for either
		// start all animations without waiting for first one to finish
		// start with the ith element in the list
		// for deletion.
		// adding index only works with up transition.

		// so i guess what ill do is create an array of elements
		var elements = [];
		if(direction === 'up') {
			// come back to this
		} else {
			elements = document.querySelectorAll('.filled');
			// iterate over nodelist
			var position = 1;
			for(var ii = 0; ii < elements.length; ++ii) {
				// position of element in its row
				// 0 1 2
				position = (ii + 1) % 3;
				// trigger animatation on this element
				// elements[ii]
			}
		}
	}
*/	
	// handles pagination in a endless scroll style
	gifs.offset = 0;
	gifs.endless = function() {
		// may not work in some browsers
		// gets the distance in pixels from the bottom 
		// of the window to the bottom of the page
		gifs.bottomoffset = document.body.scrollHeight
							- (window.innerHeight + window.pageYOffset);

		if(gifs.bottomoffset < 300) {
			$('#loading').show();
			$.ajax({
				url: 'rpc.php',
				data: {
					more: 1,
					after:gifs.offset
				},
				success: function(data) {
					setTimeout(function() {$('#loading').fadeOut()}, 100);
					data = JSON.parse(data);
					if(data.end_of_list === 1) {
						$(window).unbind('scroll');
					}
					gifs.newFrames = $(data.html_string);
					////$('.list').append(newFrames).masonry('appended', newFrames);
					$('.list').append(gifs.newFrames).masonry('reload');
					// console.log(data.img_json); // array of objects.

					// can this be done with ICH.js?
					// gifs.doc = document.createDocumentFragment();
					// for(ii = 0; ii < data.img_json.length; ii++) {
					// 	// gifs.eles.div = document.createElement('div');
					// 	// gifs.eles.div.class = 'frame filled';
					// 	// gifs.eles.div2 = document.createElement('div');
					// 	// gifs.eles.div2.class = 'inner';
					// 	// gifs.eles.a = document.createElement('a');
					// 	// gifs.eles.a.href = data.img_json.file;
					// 	// gifs.eles.a.target = '_blank';
					// 	// gifs.eles.img = document.createElement('img');
					// 	// gifs.eles.img.src = data.
					// 	gifs.ele = ich.

					// }

					// bind hover action to new frames.
					$('.filled').hoverIntent(gifs.hoverConfig);
				},
				error: function() {
					setTimeout(function() {$('#loading').fadeOut()}, 100);
					console.log('Error in endless scroll request');
					return true;
				}
			});
			// get the next 20 next time
			gifs.offset += 20;
		}
	};

	// set custom validity
	$('#url-field').blur(function() {
		if (this.validity.patternMismatch === true) {
			this.setCustomValidity('Please enter a URL that ends in .gif');
		} else {
			this.setCustomValidity('');
		}
	});

	// initialize the page.
	gifs.endless();
	// copy the form for re-insertions
	gifs.formMarkup = $('.blank-form').clone(true);
	$('.list').masonry({
		itemSelector: '.frame',
		//cornerStampSelector: '.blank-form',
		colunWidhth: 300,
		gutterWidth: 20
	});

	// bind fetch endless to scroll event
	$(window).bind('scroll', gifs.endless);
})(jQuery);