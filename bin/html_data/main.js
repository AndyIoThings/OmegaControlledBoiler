import apiSwitch from './modules/api/switch.js';
import apiTime from './modules/api/time.js';
import {getCurrentDateTime} from "./modules/utils/time.js";

globalThis.window.addEventListener('load', async () => {
    const serverTime = await apiTime.getServerTime();
    const dom = globalThis.document;

    dom.getElementById('pc-datetime').innerHTML = getCurrentDateTime();
    dom.getElementById("dev-date-time").innerHTML =  serverTime ?? '--:--';

    dom
        .getElementById('sync-button')
        .addEventListener('click', apiTime.SyncTime);

    dom
        .getElementById('switchOn')
        .addEventListener('click', apiSwitch.SwitchOn);

    dom
        .getElementById('switchOff')
        .addEventListener('click', apiSwitch.SwitchOff);
});

