export function SyncTime() {
    const xhttp = new XMLHttpRequest();
    xhttp.onload = function () {
        document.getElementById("dev-date-time").innerHTML = this.responseText;
    }
    var dt = new Date();
    var time = -dt.getTimezoneOffset() * 60000 + dt.getTime();
    console.log(dt.getTimezoneOffset());
    console.log(time);
    console.log(dt.getTime());
    var newUrl = window.location.href + 'sync_time=';
    xhttp.open("GET", newUrl + time);
    xhttp.send();

}

export const getServerTime = async () => {
    const url = `//${window.location.host}/get_date_time?t=${Math.random()}`;
    try {
        const response = await fetch(url);
        if (!response.ok) {
            // make the promise be rejected if we didn't get a 2xx response
            throw new Error("Not 2xx response", {cause: response});
        } else {
            return response.text();
        }
    } catch (err) {
        return null;
    }
}

export default {
    SyncTime,
    getServerTime
}