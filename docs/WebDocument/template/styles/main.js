
var element = document.getElementById("navbar-revision-box");

if (location.href.includes("latest-lumino-webdocs")) {
    element.style.display = "block";
}
else {
    element.style.display = "none";
}

