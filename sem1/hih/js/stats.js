let rightSideMenu = $("#rightSideMenu");
let dropBut = $("#dropBut");

rightSideMenu.css("display", "block");
dropBut.css("marginLeft", "0px");

const menu = document.getElementById("menu");
menu.style.left = "100vw";

function changeMenu(){
    if (menu.style.left == "100vw") {
        menu.style.left = "0";
    } else {
        menu.style.left = "100vw";
    }
}
function dropdownHandler(){
    let dropdown = $("#dropdown");
    let sublist = $("#sublistMenu");

    if (dropdown.css("transform")[7] == "0"){
        sublist.css("height", "auto");
        sublist.css("visibility", "visible");
        sublist.css("opacity", "1");
        dropdown.css("transform", "rotate(0deg)");
    } else {
        sublist.css("opacity", "0");
        sublist.css("height", "0");
        sublist.css("visibility", "hidden");
        dropdown.css("transform", "rotate(-90deg)");
    }
}


let lang = navigator.language;
ind = lang.indexOf("-")
var articleElement = document.querySelector("body header");
let langImg = document.createElement("img");
if (ind !== -1) {
    langImg.setAttribute("src", `https://unpkg.com/language-icons/icons/${lang.substring(0, ind)}.svg`);
} else {
    langImg.setAttribute("src", `https://unpkg.com/language-icons/icons/${lang}.svg`);
}
langImg.setAttribute("width", "30px");
langImg.setAttribute("height", "30px");
langImg.setAttribute("alt", "Country Image");
articleElement.insertBefore(langImg, articleElement.firstChild);