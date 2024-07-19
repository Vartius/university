let rightSideMenu = $("#rightSideMenu");
let dropBut = $("#dropBut");

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


window.addEventListener('resize', function(event) {
    let viewer = document.getElementById('imgViewer');
    let imgToView = document.getElementById('imgToView');

    if (viewer.style.display == "flex") {
        let imgX = imgToView.offsetWidth
        let imgY = imgToView.offsetHeight
        let winX = window.innerWidth
        let winY = window.innerHeight

        let xRate = imgX / (winX - 100);
        let yRate = imgY / (winY - 100);

        if (xRate > yRate){
            imgToView.style.width = `${Math.round(imgX/xRate)}px`;
            imgToView.style.height = `${Math.round(imgY/xRate)}px`;
        } else {
            imgToView.style.width = `${Math.round(imgX/yRate)}px`;
            imgToView.style.height = `${Math.round(imgY/yRate)}px`;
        }
    }

}, true);


function openImg(imgElement) {
    let viewer = document.getElementById('imgViewer');
    let imgToView = document.getElementById('imgToView');

    viewer.style.display = "flex";
    imgToView.src = imgElement.src;

    let imgX = imgElement.offsetWidth
    let imgY = imgElement.offsetHeight
    let winX = window.innerWidth
    let winY = window.innerHeight

    let xRate = imgX / (winX - 100);
    let yRate = imgY / (winY - 100);

    if (xRate > yRate){
        imgToView.style.width = `${Math.round(imgX/xRate)}px`;
        imgToView.style.height = `${Math.round(imgY/xRate)}px`;
    } else {
        imgToView.style.width = `${Math.round(imgX/yRate)}px`;
        imgToView.style.height = `${Math.round(imgY/yRate)}px`;
    }
}


function closeImg() {
    let viewer = document.getElementById('imgViewer');
    viewer.style.display = "none";
}

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