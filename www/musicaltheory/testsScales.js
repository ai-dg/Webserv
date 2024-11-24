import { getNeighbourTones, getRelativeMinor, getHarmScale } from "./tones.js"
import { majScale, minScale, scaleToString, displayIn } from "./scales.js";
import { putIntheCircleEdge } from "./functions.js";
import { engage_neighboor_tones_menu } from "./tonalfunctions.js";

const fdmaj = document.querySelector('.fdmaj');
const fdmin = document.querySelector('.fdmin');
const rmaj = document.querySelector('.rmaj');
const rmin = document.querySelector('.rmin');
const fumaj = document.querySelector('.fumaj');
const fumin = document.querySelector('.fumin');
const harmScale = document.querySelector('.harmScale');
const harmMinScale = document.querySelector('.harmMinScale');
const harmFuScale = document.querySelector('.harmFuScale');
const harmFuMinScale = document.querySelector('.harmFuMinScale');
const harmFdScale = document.querySelector('.harmFdScale');
const harmFdMinScale = document.querySelector('.harmFdMinScale');

const maj = [rmaj, fdmaj, fumaj];
const min = [rmin, fdmin, fumin];

const nt_notes = [...document.getElementsByClassName("nt_notes")];
const alt = [...document.getElementsByClassName("alt")];

const ntsmin = [harmMinScale, harmFdMinScale, harmFuMinScale];
const nts = [harmScale, harmFdScale, harmFuScale];
console.log(ntsmin);
console.log(nts);

engage_neighboor_tones_menu();
let nb = getNeighbourTones("G");


displayIn(scaleToString(nb.scale.maj), harmScale);
displayIn(scaleToString(nb.scale.min), harmMinScale);
displayIn(scaleToString(nb.fifthUp.maj), harmFuScale);
displayIn(scaleToString(nb.fifthUp.min), harmFuMinScale);
displayIn(scaleToString(nb.fifthDown.maj), harmFdScale);
displayIn(scaleToString(nb.fifthDown.min), harmFdMinScale);

fdmaj.innerHTML = nb.fifthDown.maj[0];
fdmin.innerHTML = nb.fifthDown.min[0] + "m";
rmaj.innerHTML = nb.scale.maj[0];
rmin.innerHTML = nb.scale.min[0] + "m";
fumaj.innerHTML = nb.fifthUp.maj[0];
fumin.innerHTML = nb.fifthUp.min[0] + "m";

putIntheCircleEdge(document.querySelector(".cir"), nts)
putIntheCircleEdge(document.querySelector(".cir2"), ntsmin)
putIntheCircleEdge(document.querySelector(".cir"), maj)
putIntheCircleEdge(document.querySelector(".cir2"), min)
putIntheCircleEdge(document.querySelector(".nt_notes_wrap"), nt_notes)


console.log(getHarmScale("C", "maj", 4))