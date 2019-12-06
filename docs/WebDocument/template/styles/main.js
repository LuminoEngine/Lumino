
console.log("my main.");
var element = document.getElementById("navbar-revision-box");
console.log(element);

console.log(location.href);
console.log(location.href.includes("localhost"));
console.log(location.href.includes("latest-lumino-webdocs"));

if (location.href.includes("latest-lumino-webdocs")) {
    element.style.display = "block";
}
else {
    element.style.display = "none";
}

