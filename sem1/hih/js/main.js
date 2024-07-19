let theta0 = 3.3;
let theta1 = 12;
let progressbarAnimationMs = 2000;

let rightSideMenu = $("#rightSideMenu");
let dropBut = $("#dropBut");

if ('scrollRestoration' in history) {
    history.scrollRestoration = 'manual';
  }
rightSideMenu.css("display", "block");
dropBut.css("marginLeft", "0px");
const menu = document.getElementById("menu");
menu.style.left = "100vw";


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

function sigmoid(s, x) {
    const exponent = -(theta0 + theta1 * (x + 0.25 - s));
    return 1 / (1 + Math.exp(exponent));
}

function sigmoidReverse(s, x) {
    const exponent = -(theta0 + theta1 * (-x + s + 0.25));
    return 1 / (1 + Math.exp(exponent));
}

function getCol(s, x, min, max) {
    let y1 = sigmoid(s, x);
    let y2 = sigmoidReverse(s, x);
    if (y1 <= sigmoid(0, 0)) {
        return min + (max - min) * y1;
    } else {
        return min + (max - min) * y2;
    }
}

function setCols(rel) {
    $("#hrStart").css("boxShadow", `0 0 15px ${getCol(0, rel, 4, 6)}px rgb(16, 0, ${getCol(0, rel, 132, 234)})`);
    $("#hrFirstProgram").css("boxShadow", `0 0 15px ${getCol(1, rel, 4, 6)}px rgb(16, 0, ${getCol(1, rel, 132, 234)})`);
    $("#hrUnity").css("boxShadow", `0 0 15px ${getCol(2, rel, 4, 6)}px rgb(16, 0, ${getCol(2, rel, 132, 234)})`);
    $("#hrAttempts").css("boxShadow", `0 0 15px ${getCol(3, rel, 4, 6)}px rgb(16, 0, ${getCol(3, rel, 132, 234)})`);
    $("#hrContacts").css("boxShadow", `0 0 15px ${getCol(4, rel, 4, 6)}px rgb(16, 0, ${getCol(4, rel, 132, 234)})`);
}

setCols(0);
$(window).on("scroll", function () {
    let rel = $(window).scrollTop() / (window.innerHeight + 50);
    setCols(rel);
});

function changeMenu(){
    if (menu.style.left == "100vw") {
        menu.style.left = "0";
    } else {
        menu.style.left = "100vw";
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


$("#progressbarContainer").css("display", "flex")
if (!localStorage["loaded"]) {
    localStorage["loaded"] = 0;
}
if (localStorage["loaded"] == 0) {
    for (let i = 0; i <= 100; i++) {
        setTimeout(function() {
            $( "#progressbar" ).progressbar({
                value: i
            });
        }, progressbarAnimationMs/100*i);
    }
    setTimeout(function() {
        $( "#progressbarContainer" ).hide( "fade", {}, 1000 );
        localStorage["loaded"] = 1;
    }, progressbarAnimationMs*.6);
} else {
    $( "#progressbar" ).progressbar({
        value: 0
    });
    setTimeout(function() {
        $( "#progressbar" ).progressbar({
            value: 65
        });
    }, 100);
    setTimeout(function() {
        $( "#progressbar" ).progressbar({
            value: 100
        });
        $( "#progressbarContainer" ).hide( "fade", {}, 1000 );
    }, 200);
}
