function searchAndHighlight() {
	alert('Hi!');
	$('body').removeHighlight();
	var searchTerm = document.getElementById("search").value;
	$('body').highlight(searchTerm);
}