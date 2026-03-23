/* LINE Solver Documentation - Shared JavaScript */

$(document).ready(function(){
	// Load common sidebar content
	$('#sidebar-content').load('sidebar.html', function() {
		// After sidebar loads, highlight the active page
		highlightActivePage();
	});

	// Highlight active page in navigation
	function highlightActivePage() {
		var currentPage = window.location.pathname.split('/').pop() || 'index.html';

		// Remove any existing active class
		$('.sidebar-nav li').removeClass('active');

		// Add active class to current page
		$('.sidebar-nav a').each(function() {
			var href = $(this).attr('href');
			if (href === currentPage ||
				(currentPage === 'index.html' && href === 'index.html') ||
				(currentPage === '' && href === 'index.html')) {
				$(this).parent().addClass('active');
			}
		});
	}

	// Highlight sidebar item based on URL hash
	function highlightSidebarHash(hash) {
		$('.sidebar-nav .submenu li').removeClass('active');
		if (hash) {
			$('.sidebar-nav .submenu a[href="' + hash + '"]').parent().addClass('active');
		}
	}

	// Highlight on page load if hash is present
	if (window.location.hash) {
		highlightSidebarHash(window.location.hash);
	}

	// Update highlight when hash changes (back/forward navigation)
	$(window).on('hashchange', function() {
		highlightSidebarHash(window.location.hash);
	});

	// Update highlight when clicking submenu hash links
	$(document).on('click', '.sidebar-nav .submenu a[href^="#"]', function(){
		highlightSidebarHash($(this).attr('href'));
	});
});
