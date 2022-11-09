export function SwitchOn() {
    const xhttp = new XMLHttpRequest();
    xhttp.onload = function () {
        var respArray = this.responseText.split(':');
        if (respArray[1] === "Ok")
            document.getElementsByName("switchOn")[0].setAttribute("disabled", "disabled");
        document.getElementsByName("switchOff")[0].removeAttribute("disabled");
        var a = document.getElementsByName("switchOn")[0].classList;
        a.remove("switch");
        a.add("switchInactive");
        document.getElementsByName("switchOff")[0].classList.remove("switchInactive");
        document.getElementsByName("switchOff")[0].classList.add("switch");
    }
    xhttp.open("GET", window.location.href + "switch=On");
    xhttp.send();
}

export function SwitchOff() {
    const xhttp = new XMLHttpRequest();
    xhttp.onload = function () {
        var respArray = this.responseText.split(':');
        if (respArray[1] === "Ok") {
            document.getElementsByName("switchOff")[0].setAttribute("disabled", "disabled");
            document.getElementsByName("switchOn")[0].removeAttribute("disabled");
            document.getElementsByName("switchOn")[0].classList.remove("switchInactive");
            document.getElementsByName("switchOn")[0].classList.add("switch");
            document.getElementsByName("switchOff")[0].classList.remove("switch");
            document.getElementsByName("switchOff")[0].classList.add("switchInactive");
        }
    }
    xhttp.open("GET", window.location.href + "switch=Off");
    xhttp.send();
}

export default {
    SwitchOn,
    SwitchOff
}